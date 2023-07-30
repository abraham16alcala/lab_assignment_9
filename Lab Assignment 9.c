#include <stdio.h>
#include <stdlib.h>



// RecordType
struct RecordType
{
	int id;
	char name;
	int order; 
};

// Node for separate chaining
struct Node
{
	struct RecordType data;
	struct Node* next;
};

// HashType
struct HashType
{
	struct Node* head;
};

// Compute the hash function
int hash(int x)
{
	return abs(x) %15;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i = 0; i < hashSz; ++i)
	{
		struct Node* current = pHashArray[i].head;
		if (current != NULL)
		{
			printf("index %d -> ", i);
			while (current != NULL)
			{
				printf("%d, %c, %d -> ", current->data.id, current->data.name, current->data.order);
				current = current->next;
			}
			printf("NULL\n");
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Your hash implementation
	struct HashType pHashArray[15];

	// Initialize the hash table
	for (int i = 0; i < 15; i++)
	{
		pHashArray[i].head = NULL;
	}

	// Insert records into the hash table
	for (int i = 0; i < recordSz; i++)
	{
		int hashValue = hash(pRecords[i].id);
		struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
		if (newNode == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		newNode->data = pRecords[i];
		newNode->next = pHashArray[hashValue].head;
		pHashArray[hashValue].head = newNode;
	}

	// Display records in the hash table
	displayRecordsInHash(pHashArray, 15);

	// Free memory
	free(pRecords);
	for (int i = 0; i < 15; i++)
	{
		struct Node* current = pHashArray[i].head;
		while (current != NULL)
		{
			struct Node* next = current->next;
			free(current);
			current = next;
		}
	}

	return 0;
}
