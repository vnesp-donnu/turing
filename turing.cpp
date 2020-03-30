#include <cstdio>
#include <cstring>

const int MAX_TIME = 1000;
const int MaxQSize = 100;
const int MaxASize = 100;
const int MaxTapeLen = 10000;

struct DeltaValue {
	char newChar;
	int newState;
	short shift;
	
	void read(int defaultState, char defaultChar) {
		newChar = defaultChar;
		shift = 0;
		newState = defaultState;
		
		char buffer[100];
		
		scanf("%s", buffer);
		char *s = buffer;
		if (s[0] == '-' && s[1] == 0) {
			newState = -1;
			return;
		}
		
		if (*s != ',')
			newChar = *s++;
		
		if (*s != ',') {
			printf("Invalid char in state %d for char %c", defaultState, defaultChar);
			return;
		}
		
		s++;
		if (*s != ',') {
			shift = (*s == 'R') - (*s == 'L');
			s++;
		}

		if (*s != ',') {
			printf("Invalid shift in state %d for char %c", defaultState, defaultChar);
			return;
		}

		s++;
		if (*s)
			sscanf(s, "%d", &newState);
		
		// printf("%c,%d,%d\t", newChar, shift, newState);
	}
	
	void print() const {
		printf("newChar = '%c', shift = %hd, newState = %d\n", newChar, shift, newState); 
	}
};

struct Tape {
	char space[2 * MaxTapeLen + 1];
	char *strip;
	int leftBound, rightBound;
	int pos;
	
	Tape() {
		strip = space + MaxTapeLen;
	}
	
	void read(char emptyChar) {
		scanf("%d%d", &leftBound, &rightBound);
		for (int i = leftBound; i <= rightBound; i++)
			strip[i] = emptyChar;
		scanf("%d", &pos);
		scanf("%s", strip + pos);
		strip[pos + strlen(strip + pos)] = emptyChar;
		scanf("%d", &pos);
	}
	
	char get() {
		return strip[pos]; 
	}
	
	void put(char c) {
		strip[pos] = c;
	}
	
	bool move(int shift) {
		pos += shift;
		return (leftBound <= pos && pos <= rightBound);
	}

	void print() {
		for (int i = leftBound; i <= rightBound; i++) {
			putchar(strip[i]);
		}
		putchar('\n');
		for (int i = leftBound; i <= rightBound; i++) {
			putchar((i==pos) ? '^' : ' ');
		}
		putchar('\n');
	}
	
};

struct TuringMachine {
	int QSize;
	int ASize;
	char A[MaxASize+1];
	DeltaValue delta[MaxQSize][MaxASize];
	int invA[256];
	Tape tape;
	int state;
	
	void makeinvA() {
		for (int i = 0; i < 256; i++)
			invA[i] = -1;
		unsigned char c;
		for (int i = 0; c = A[i], c; i++)
			invA[c] = i;
	}
	
	void read() {
		scanf("%d", &QSize);
		scanf("%s", A);
		// puts(A);
		ASize = strlen(A);
		makeinvA();
		for (int q = 0; q < QSize; q++)
			for (int i = 0; i < ASize; i++)
				delta[q][i].read(q, A[i]);
	}
	
	void init() {
		state = 0;
	}

	void readTape() {
		tape.read(A[0]);
	}

	bool stepOver(bool debug) {
		unsigned char c = tape.get();
		if (debug) {
			tape.print();
			printf("state = %d currentChar = '%c'\n", state, c);
		}
		int i = invA[c];
		if (i < 0 || i >= ASize) {
			if (debug)
				puts("Unknown char");
			return false;
		}
		const DeltaValue& cur = delta[state][i];
		if (debug)
			cur.print();
		tape.put(cur.newChar);
		if (!tape.move(cur.shift)) {
			puts("Position ouf of range");
			return false;
		}
		state = cur.newState;
		if (state < 0 || state > QSize) {
			puts("Invalid state");
			return false;
		}
		return true;
	}
	
	bool run(bool debug) {
		int time = 0;
		init();
		while (0 <= state && state < QSize) {
			time++;
			if (time > MAX_TIME) {
				puts("Time Limit Exceeded");
				break;
			}
			if (debug)
				printf("Turn #%d:\n", time);
			if (!stepOver(debug))
				return false;
		}
		return true;
	}
	
	void readAndRun(bool debug) {
		readTape();
		puts("Ininial Tape");
		tape.print();
		if (run(debug)) {
			puts("Final Tape");
			tape.print();
		}
		else
			puts("Error!");
	}
};

TuringMachine machine;

int main(int argc, char** argv) {

	bool debug = argc > 1 && strcmp(argv[1], "-d")==0;

	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
	machine.read();

	int NTest;
	scanf("%d", &NTest);
	for (int iTest = 1; iTest <= NTest; iTest++) {
		printf("Test %d:\n", iTest);
		machine.readAndRun(debug);
	}
}
