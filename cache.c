#include <stdio.h>
#include <stdlib.h>

typedef unsigned char *byte_pointer;

struct block{
	unsigned char valid;
	unsigned int tag;
	unsigned int value;
};

unsigned int getSetNumber(unsigned int address){
	int mask = 0x0000003c;
	int setNum = (address & mask) >> 2;
	//return set number
	return setNum;
}

unsigned int getOffset(unsigned int address){
	int mask = 0x00000003;
	int offSet = address & mask;
	//return the offset
	return offSet;
}

unsigned int getTag(unsigned int address){
	int mask = 0xffffffc0;
	int tag = (address & mask) >> 6;
	//return the tag
	return tag;
}

//show_bytes used to print the values of the byte_pointer
void show_bytes(byte_pointer start, int len){
	int i;
	for (i = 0; i < len; i++)
		printf(" %.2x", start[i]);
	printf("\n");
}

void show_bit(unsigned int x){
	//sizeof short int will be 4 nibbles long
	return show_bytes((byte_pointer) &x, sizeof(unsigned int));
}

unsigned int giveOffsetindex(unsigned int x, unsigned int off){
	int right = x >> (24 - (8 * (3 - off)));
	int mask = right & 0x000000FF;
	
	return mask;
	
}

int main(int argc, char **argv)
{
	//struct block *cache = allocate memory
	struct block *cache = malloc(sizeof(struct block)* 16);
	
	//initialize cache
	for (unsigned int i; i < 16; i++){
		cache[i].valid = 0;
	}
	
	char c;
	
	do {
		printf("Enter 'r' for read, 'w' for write, 'p' to print, 'q' to quit: \n");
		scanf(" %c", &c);
		
		//read
		if(c == 'r'){
			printf("Enter 32-bit unsigned hex addess: \n");
			unsigned int a;
			scanf("%x", &a);

			//returns values of set
			unsigned int setNumber = getSetNumber(a);
			unsigned int offset = getOffset(a);
			unsigned int tag = getTag(a);
			
			printf("looking for set: %x - tag: %x \n", setNumber, tag);
		
			//block is valid
			if(cache[setNumber].valid != 0){
				//retreive the block value
				//print out all information
				//"found set: - tag: - offset: - valid - value: "
				printf("found! set: %d - tag: %d - offset: %d valid: %d - value:", setNumber, cache[setNumber].tag, offset, cache[setNumber].valid);
				show_bit(giveOffsetindex(cache[setNumber].value, offset));
				//tags match
				if(tag == cache[setNumber].tag){
					printf("hit! \n");
				} else{
					printf("Tags don't match - Miss! \n");
				}
			} else{
				printf("no valid set found - Miss!\n");
			}
		}
		//write
		else if (c == 'w'){
			
			unsigned int add;
			unsigned int val;
			
			printf("Enter 32-bit unsigned hex address: \n");
			scanf("%x", &add);
			
			printf("Enter 32-bit unsigned hex value: \n");
			scanf("%x", &val);
			
			unsigned int setNumber = getSetNumber(add);
			unsigned int tag = getTag(add);
			
			//block is valid, and needs to be evicted
			if(cache[setNumber].valid != 0){
				//retreive the current block value;
				//print out all information: set, index, tag of new block;
				printf("evicting block - set: %d tag: %d valid: %d value:", setNumber, cache[setNumber].tag, cache[setNumber].valid);
				show_bit(cache[setNumber].value);
			}
			
			//block -> value = 
			//block -> tag = 
			//block -> valid =
			cache[setNumber].value = val;
			cache[setNumber].tag = tag;
			cache[setNumber].valid = 0x1;
			
			printf("wrote set: %d - tag: %d - valid: %d - value:", setNumber, tag, cache[setNumber].valid);
			show_bit(cache[setNumber].value);
			
			//retreive the value we write
			//print out all information
		} 
		// print
		else if (c == 'p'){
			for (unsigned int i = 0; i < 16; i++){
				//block is valid
				if(cache[i].valid != 0){
					//retreive the value of the current block
					//print out all information
					printf("set: %d - tag: %d - valid: %d - value:", i ,cache[i].tag ,cache[i].valid);
					show_bit(cache[i].value);
				}
			}
		}
	} while (c != 'q'); // not equal to quit
	return 0;
}

