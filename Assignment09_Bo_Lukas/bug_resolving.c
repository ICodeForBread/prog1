// Author: Boyue, Lukas
/*
Compile: make spacecrafts
Run: ./spacecrafts
Compile & run: make spacecrafts && ./spacecrafts
*/

#include "base.h"

#define N 5

// struct of a spacecraft
typedef struct Spacecraft{
	char* name;
	int passengers;
	double reach;
	double load_capacity;	
}Spacecraft;

// struct of the fleet. 
typedef struct Fleet{
	Spacecraft** ships;
	int maximum_count; //initialize to N, size of ships
	int current_count; //current count of spacecrafts in ships
}Fleet;
	

// returns a pointer to a dynamically allocated ship
Spacecraft* new_spacecraft(int passengers, double reach, double load_capacity, char* name){
	Spacecraft *spc = xmalloc(sizeof(Spacecraft));
	spc->passengers = passengers;
	spc->reach = reach;
	spc->load_capacity = load_capacity;
	spc->name = s_copy(name);
	free(name);
	return spc;
}

// prints a spacecraft
void print_spacecraft(Spacecraft* spc){
	printf("%12s with %3d passengers, a reach of %8.1f au and a load capacity of %4.1f t\n", spc->name, spc->passengers, spc->reach, spc->load_capacity);
}

// helper
/*These helper can not be used in the main function beacause these functions read the values in a different way then the functions in main*/
int countShipsInString(char* s) {
	int ships = -1;
	while (*s != '\0'){
		if(*s == '\n')
			ships++;
		s++;
	}
	return ships;
}

char* getToken(char* tokenStart, int lengthOfToken) {
	char* token = xmalloc(lengthOfToken + 1);
	token[lengthOfToken] = '\0';
	for (int i = 0; i < lengthOfToken; ++i) {
		token[i] = *tokenStart;
		tokenStart++;
	}
	return token;
}

char* toStartOfToken(char* s) {
	while(*s == ' ') {
		s++;
	}
	return s;
}

char* toEndOfToken(char* s) {
	while(*s != ' ' && *s != '\n') {
		s++;
	}
	return s;
}

void pointerToToken(char** s, char** tokenStartPointer) {
	*s = toStartOfToken(*s); // to start of next token
	*tokenStartPointer = *s;
	*s = toEndOfToken(*s); // to end of token
}

// imports spacecrafts from file.
Fleet* read_spacecrafts(char* file_name){
	char* file = s_read_file(file_name);
	char* s = file;
	int ships = countShipsInString(s); // number of ships already in fleet
	
	// allocate memory for the fleet
	Fleet *fleet = xmalloc(sizeof(Fleet)); //before the change: malloc(sizeof(Fleet)). Reason: We use xmalloc instead of malloc.
	fleet->current_count = ships;
	fleet->maximum_count = N;
	fleet->ships = xcalloc(N, sizeof(Spacecraft*)); //before the change: xcalloc(N, sizeof(Spacecraft)). Reason: We only need N * sizeof(Spacecraft*). Allocated too much memory.
	
	while(*s != '\0'){
		Spacecraft *spc = NULL;

		//skip until line end
		while (*s != '\n') s++;
		s++;
		
		// ends loop if file is done
		if(*s == '\0') break;

		// start of name
		char* tokenStartPointer = s; // point to start of token
		s = toEndOfToken(s); // point to end of token
	
		char* token = getToken(tokenStartPointer, s - tokenStartPointer);
		char* name = s_copy(token);
		free(token);
	
		//---------------
		pointerToToken(&s, &tokenStartPointer);
	
		token = getToken(tokenStartPointer, s - tokenStartPointer);
		int passengers = i_of_s(token);
		free(token);

		//---------------
		pointerToToken(&s, &tokenStartPointer);
		
		token = getToken(tokenStartPointer, s - tokenStartPointer);
		double reach = d_of_s(token);
		free(token);
		
		//---------------
		pointerToToken(&s, &tokenStartPointer);
		
		token = getToken(tokenStartPointer, s - tokenStartPointer);
		double load_capacity = d_of_s(token);
		free(token);

		//---------------
		spc = new_spacecraft(passengers, reach, load_capacity, name);
		//print_spacecraft(spc);

		fleet->ships[ships-1] = spc;
		ships--;
	}
	free(file);
	return fleet;
}

// prints the fleet.
void print_spacecrafts(Fleet* f){
	for(int i = 0; i< f->maximum_count; i++){
		if(f->ships[i]){
			printf("%2d\t", i);
			Spacecraft* spc = f->ships[i];
			printf("%12s with %3d passengers, a reach of %8.1f au and a load capacity of %4.1f t\n", spc->name, spc->passengers, spc->reach, spc->load_capacity);
			
		}
	}
}

// Entry point with command line arguments.
int main(int argc, String argv[]) {
    report_memory_leaks(true);

	Fleet* fleet = NULL;

	if(argc > 1){
		fleet = read_spacecrafts(argv[1]);
	}else{
		fleet = read_spacecrafts("spacecrafts.txt");
	}

	char buffer[256]; 
	for(int i = 0; i< 256; i++){
		buffer[i] = '\0';
	}

	int c = '\0'; 
	printf("Your fleet: \n");
	print_spacecrafts(fleet);
	
	while((c = getchar()) != 'q'){
		if (c == 's'){
			/*Its not solved in a way that makes sense. when sending a ship away the other ships stay where they are.
			the ships, after the ship that was send away, should move one space to the front

			it makes sense to check if the ship exists, otherwise it can lead to segmentation falts or the last ship will be deletet
			*/
			//send out a ship, remove it from the fleet
			//example: s1
			//removes ship of index 1
			int index = getchar() - '0';
			if (index >= fleet->current_count) {
				printf("tried to send ship away that doesent exist\n");
			} else {
				free(fleet->ships[index]->name);
				free(fleet->ships[index]);
				for (int i = index; i < fleet->current_count - 1; ++i) {
					fleet->ships[i] = fleet->ships[i + 1];
				}
				fleet->ships[fleet->current_count - 1] = NULL;
				fleet->current_count--;
			}
		}else if (c == 'n'){
			//change name of ship
			//example: n1 Enterprise
			//renames ship at index 1 to Enterprise
			int index = getchar() - '0';
			getchar();
			int i = 0;
			while ((c = getchar()) != '\n'){
				buffer[i] = c;
				i++;
			}
			buffer[i] = '\0';
			free(fleet->ships[index]->name);
			fleet->ships[index]->name = s_copy(buffer);
		}else if (c == 'a'){
			//add ship
			//example: a Amagedon 34 500.3 2000.0
			if (fleet->current_count == fleet->maximum_count) {
				printf("the maximum ammount of %d ships has been reached.\n", fleet->maximum_count);
			} else {
				//create string form input
				getchar();
				int i = 0;
				while ((c = getchar()) != '\n'){
					buffer[i] = c;
					i++;
				}
				buffer[i] = '\0';
				
				char* name = NULL;
				i = 0;
				for(i = 0; i< s_length(buffer); i++){
					if(buffer[i] == ' '){
						break;
					}
				}
				
				name = s_sub(buffer, 0, i);
				i++;
				int j = i;
				printf("Char: '%c'\n", buffer[j]);
				for(; j < s_length(buffer); j++){
					if(buffer[j] == ' '){
						break;
					}
				}
				char* passengersString = s_sub(buffer, i, j);
				printf("String: '%s' \n", passengersString);
				
				int passengers = i_of_s(passengersString);
				free(passengersString);
				i = j++;
				for(; j< s_length(buffer); j++){
					if(buffer[j] == ' '){
						break;
					}
				}
				char* reachString = s_sub(buffer, i, j);
				double reach = d_of_s(reachString);
				free(reachString);
				i = j++;
				for(; j < s_length(buffer); j++){
					if(buffer[j] == ' '){
						break;
					}
				}
				
				char* loadString = s_sub(buffer, i, j);
				double load_capacity =  d_of_s(loadString);
				free(loadString);

				Spacecraft* spc = new_spacecraft(passengers, reach, load_capacity, name);
				fleet->ships[fleet->current_count] = spc;
				fleet->current_count++;
				
			}
		}
		if(c == '\n'){
			printf("Your fleet: \n");
			print_spacecrafts(fleet);
		}
		
	}
	for (int i = 0; i < fleet->current_count; ++i) {
		free(fleet->ships[i]->name);
		free(fleet->ships[i]);
	}
	free(fleet->ships);
	free(fleet);

    return 0;
}
