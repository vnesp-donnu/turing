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
	
	bool read(int defaultState, char defaultChar, bool debug) {
		newChar = defaultChar;
		shift = 0;
		newState = defaultState;
		
		char buffer[100];
		
		scanf("%s", buffer);
		if (debug)
			printf("%d, %c -> %s\n", defaultState, defaultChar, buffer);
		char *s = buffer;
		if (s[0] == '-' && s[1] == 0) {
			newState = -1;
			return true;
		}
		
		if (*s != ',')
			newChar = *s++;
		
		if (*s != ',') {
			printf("Invalid char in state %d for char %c", defaultState, defaultChar);
			return false;
		}
		
		s++;
		if (*s != ',') {
			shift = (*s == 'R') - (*s == 'L');
			s++;
		}

		if (*s != ',') {
			printf("Invalid shift in state %d for char %c", defaultState, defaultChar);
			return false;
		}

		s++;
		if (*s) {
			if (*s == 'q')
				s++;
			if (sscanf(s, "%d", &newState) != 1 || newState < 0) {
				printf("Invalid state in state %d for char %c", defaultState, defaultChar);
				return false;
			}
		}
		
		// printf("%c,%d,%d\t", newChar, shift, newState);
		return true;
	}
	
	void print() const {
		printf("newChar = '%c', shift = %hd, newState = %d\n", newChar, shift, newState); 
	}
};

struct Answer {
	int pos;
	char strip[2 * MaxTapeLen + 1];
	
	void read() {
		scanf("%d", &pos);
		scanf("%s", strip);
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
	
	bool check(const Answer& ans) {
		int len = strlen(ans.strip);
		int shift = pos - ans.pos;
		return strncmp(ans.strip, strip + shift, len) == 0;
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
	Answer ans;
	int state;
	
	void makeinvA() {
		for (int i = 0; i < 256; i++)
			invA[i] = -1;
		unsigned char c;
		for (int i = 0; c = A[i], c; i++)
			invA[c] = i;
	}
	
	bool read(bool debug) {
		scanf("%d", &QSize);
		scanf("%s", A);
		// puts(A);
		ASize = strlen(A);
		makeinvA();
		for (int q = 0; q < QSize; q++)
			for (int i = 0; i < ASize; i++)
				if (!delta[q][i].read(q, A[i], debug))
					return false;
		return true;
	}
	
	void init() {
		state = 0;
	}

	void readTape() {
		tape.read(A[0]);
	}
	
	void readAnswer() {
		ans.read();
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
			puts("Position out of range");
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
				return false;
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
		readAnswer();
		puts("Ininial Tape");
		tape.print();
		if (run(debug)) {
			puts("Final Tape");
			tape.print();
			puts(tape.check(ans) ? "OK!" : "Wrong Answer");
		}
		else
			puts("Error!");
		puts("");
	}
};

TuringMachine machine;

int main(int argc, char** argv) {

	if (argc < 3) {
		printf("Usage: turing <input_file> <output_file> [option]\n");
		printf("Option:\n");
		printf("  -d -- output debug information\n");
		return -1;
	}
	
	if (freopen(argv[1], "r", stdin) == NULL) {
		printf("File %s: reading error", argv[1]);
		return -1;
	}
	
	if (freopen(argv[2], "w", stdout) == NULL) {
		printf("File %s: creating error", argv[2]);
		return -1;
	}

	bool debug = argc > 3 && strcmp(argv[3], "-d")==0;
	
	if ( machine.read(debug) ) {
		int NTest;
		scanf("%d", &NTest);
		for (int iTest = 1; iTest <= NTest; iTest++) {
			printf("Test %d:\n", iTest);
			machine.readAndRun(debug);
		}
	}
	
}
