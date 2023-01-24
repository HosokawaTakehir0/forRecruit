#include <iostream>
#include<fstream>
#include <string>
#include <chrono>
#include <random>
#include <vector>
#include<filesystem>

#define ErrorProb 0  //about 0.8, almost equal strong
#define GAINMYCOE 1.0//1.0:2.0 or 2.0:1.0@is good(?)
#define GAINOPCOE 1.0//1.0:2.0 or 2.0:1.0@is good(?)
/*
{turnBoxes:240,971,868 byte
,treeLight:133,873,260 byte
,treeHeavy:776,464,908byte}*/

//if you want com to study,please un ommnetout
//#define STUDY		
//#define STRONG

#ifdef STUDY
	void study1();

	#ifdef STRONG
	void creTreeSt();
	#define study2() creTreeSt();

#else

	void creTree();
	#define study2() creTree();
#endif

#define act() study1();study2();

#else
void play();
#define act() play()
#endif
int main() {
	act();
	system("pause");
	return 0;
}

struct Boxes
{
	/*next_turn ::
		most significant bit	:: is game finished(0 or 1)
		2,3, bit				:: box number(0,1,2)
		least significant bit	:: who is next turn (0 or 1)
	*/
	unsigned char next_turn;
	unsigned char box[8];
};
struct Datum {
	int loc;
	unsigned char nextWin; //{not_exist(0,1,2),dummy,0,1,2,turn}
};
struct DatumExpansion {
	int loc;
	unsigned char nextWin; //{not_exist(0,1,2),dummy,0,1,2,turn}
	unsigned int WinNum[6];
};
// show board
void show(unsigned char box[8], char turn) {

	std::cout << "==============================" << std::endl;
	std::cout << "         turn" << +turn << std::endl;
	std::cout << "***************************" << std::endl;
	std::cout << " first:" << +2 << " : " << +1 << " : " << +0 << std::endl;
	std::cout << "       " << +box[2] << " : " << +box[1] << " : " << +box[0] << std::endl;
	std::cout << "    " << +box[3] << "              " << +box[7] << std::endl;
	std::cout << "       " << +box[4] << " : " << +box[5] << " : " << +box[6] << std::endl;
	std::cout << "second:" << +0 << " : " << +1 << " : " << +2 << std::endl;
	std::cout << "***************************" << std::endl;
	std::cout << "==============================" << std::endl;
	std::cout << std::endl;

}
std::mt19937 rnd{ std::random_device{}() };
std::uniform_real_distribution<float> uni(0, 1);


class Com1 {
	Datum datum;
	char inFileName[32];
	char cntBefMyTurn;
	std::vector<char> moves;
public:
	Com1();
	void renew(char x, char existBox, int turnCnt);
	char choice(char turn, char existBox);
};
class Com2 {
	DatumExpansion datum;
	char inFileName[32];
	std::vector<char> moves;
public:
	Com2();
	void renew(char x, char existBox, int turnCnt);
	char choice(char turn, char existBox);
};

void play() {
	bool isCom[2] = { 1,1 };
	//int comLv;
	Boxes nowState;
	char turnCnt;
	char num;
	char target;
	char turn;
	char x;
	char existBox;
	std::vector<char> moves;
	int cntWin = 0;
	int cntTurnAve = 0;
	std::ifstream::pos_type pos_end;
	bool contin = true;
	/*
	int cntWin0=0;
	int cntWin1=0;
	int cntTurnAve = 0;
	*/
	while (contin) {

		//board initialization
		nowState.next_turn = 0;
		for (int i = 0; i < 8; ++i) {
			nowState.box[i] = 3;
		}
		nowState.box[3] = 0;
		nowState.box[7] = 0;
		turnCnt = 0;
		turn = 0;

		//ask using com
		std::cout << "first is com?(true=1,false=0):";
		std::cin >> isCom[0];
		std::cout << "second is com?(true=1,false=0):";
		std::cin >> isCom[1];

		#ifdef STRONG
			Com2 com1;
		#else
			Com1 com1;
		#endif
		//Com2 com1;//or Com2

		//main game
		std::cout << "===============================" << std::endl;
		while (1) {

			show(nowState.box, turnCnt);

			//process changing by turn
			turn = nowState.next_turn << 2;
			existBox = 0;
			if (nowState.box[turn]) existBox = 1;
			existBox <<= 1;
			if (nowState.box[turn + 1]) existBox += 1;
			existBox <<= 1;
			if (nowState.box[turn + 2]) existBox += 1;

			//com turn
			if (isCom[nowState.next_turn]) {

				x = com1.choice(turn, existBox);
				std::cout << "com" << +nowState.next_turn << ":" << +x << std::endl;
			}
			else {
				//player turn
				while (1) {
					std::cout << "player" << +nowState.next_turn << ":";
					std::cin >> x;
					x -= '0';
					if (x < 0 || 2 < x)continue;
					if (nowState.box[turn + x])break;
					std::cin.clear();
					std::cin.ignore();
				}
			}

			//board update
			target = turn + x;
			moves.push_back(target);
			num = nowState.box[target];
			nowState.box[target] = 0;
			++target;
			for (int num2 = num; num2 > 0; --num2) {
				nowState.box[target] += 1;
				++target;
				if (target == 8)
					target = 0;
			}
			if ((target & 0b0011)) nowState.next_turn = (~nowState.next_turn) & 0b0001;

			//check finish game
			if (nowState.box[turn] | nowState.box[turn + 1] | nowState.box[turn + 2]);
			else break;
			++turnCnt;
			com1.renew(x, existBox, turnCnt);

		}

		//show last board
		std::cout << "==============================" << std::endl;
		std::cout << "***************************" << std::endl;
		std::cout << "    " << +nowState.box[2] << " : " << +nowState.box[1] << " : " << +nowState.box[0] << std::endl;
		std::cout << +nowState.box[3] << "            " << +nowState.box[7] << std::endl;
		std::cout << "    " << +nowState.box[4] << " : " << +nowState.box[5] << " : " << +nowState.box[6] << std::endl;
		std::cout << "***************************" << std::endl;
		std::cout << "==============================" << std::endl;
		std::cout << std::endl;
		turn >>= 2;
		if (isCom[turn])std::cout << "com" << +(turn) << " win" << std::endl;

		//check to start game again
		else std::cout << "player" << +(turn) << " win" << std::endl;
		std::cout << std::endl;
		std::cout << "==============================" << std::endl;


		std::cout << "Continue?(true=1,false=0):";
		std::cin >> contin;
	}
	//std::cout << "Win0:" << cntWin0 << "  Win1:" << cntWin1 
	//<<"  turnAve:"<<(double)cntTurnAve/(cntWin0+cntWin1)<< std::endl;

}

//constructor
Com1::Com1() {
	sprintf_s(inFileName, "treeLight/tree0");
	std::ifstream tree1(inFileName, std::ios::binary);
	tree1.read((char*)&datum, sizeof(char[5]));
	tree1.close();
}
Com2::Com2() {
	sprintf_s(inFileName, "treeLight/tree0");
	std::ifstream tree1(inFileName, std::ios::binary);
	tree1.read((char*)&datum, sizeof(char[29]));
	tree1.close();
}


//next turn's file load
void Com1::renew(char x, char existBox, int turnCnt) {
	int loc = datum.loc;
	for (int i = 0; i < x; ++i) {
		if (((existBox >> (2 - i)) & 1) == 1)  ++loc;
	}
	sprintf_s(inFileName, "treeLight/tree%d", turnCnt);
	std::ifstream inData1(inFileName, std::ios::binary);
	inData1.seekg(loc * 5, std::ios_base::beg);
	inData1.read((char*)&datum, sizeof(char[5]));
	inData1.close();
}
void Com2::renew(char x, char existBox, int turnCnt) {
	int loc = datum.loc;
	for (int i = 0; i < x; ++i) {
		if (((existBox >> (2 - i)) & 1) == 1)  ++loc;
	}
	sprintf_s(inFileName, "treeHeavy/tree%d", turnCnt);
	std::ifstream inData1(inFileName, std::ios::binary);
	inData1.seekg(loc * 29, std::ios_base::beg);
	inData1.read((char*)&datum, sizeof(char[29]));
	inData1.close();
}

char Com1::choice(char turn, char existBox) {
	char tmpDec;
	char x;
	if (turn) {//non exist flag=1  turn1 1   turn0 0 however goto 1
		tmpDec = ((~datum.nextWin) >> 5) & (datum.nextWin >> 1);
	}
	else {
		tmpDec = ((~datum.nextWin) >> 5) & ((~datum.nextWin) >> 1);
	}
	tmpDec &= 0x07;
	if (uni(rnd) < ErrorProb) tmpDec = 0x08;
	switch (tmpDec) {//box_num 0,1,2
	case 0x00: case 0x08:
		switch (existBox) {//box_num 0,1,2
		case 0b0001:x = 2; break; case 0b0010:x = 1; break; case 0b0100:x = 0; break;
		case 0b0011:x = rnd() % 2 + 1; break;
		case 0b0101:x = rnd() % 2 << 1; break;
		case 0b0110:x = rnd() % 2; break;
		case 0b0111:
			x = rnd() % 3;
			break;
		}
		break;
	case 0b0001:x = 2; break; case 0b0010:x = 1; break; case 0b0100:x = 0; break;
	case 0b0011:x = rnd() % 2 + 1; break;
	case 0b0101:x = rnd() % 2 << 1; break;
	case 0b0110:x = rnd() % 2; break;
	case 0b0111:
		x = rnd() % 3;
		break;
	}
	return x;
}

char Com2::choice(char turn, char existBox) {
	char tmpDec;
	char x;
	if (turn) {
		tmpDec = ((~datum.nextWin) >> 5) & (datum.nextWin >> 1);
	}
	else {
		tmpDec = ((~datum.nextWin) >> 5) & ((~datum.nextWin) >> 1);
	}
	tmpDec &= 0x07;
	if (uni(rnd) < ErrorProb) tmpDec = 0x08;
	char tnmy, tnop;
	tnmy = turn >> 2;
	tnop = (~tnmy) & 1;
	double gain[3];
	gain[0] = GAINMYCOE * datum.WinNum[tnmy] - GAINOPCOE * datum.WinNum[tnop];
	gain[1] = GAINMYCOE * datum.WinNum[tnmy + 0b0010] - GAINOPCOE * datum.WinNum[tnop + 0b0010];
	gain[2] = GAINMYCOE * datum.WinNum[tnmy + 0b0100] - GAINOPCOE * datum.WinNum[tnop + 0b0100];
	if (!(existBox & 0b0100)) gain[0] = -FLT_MAX;
	if (!(existBox & 0b0010)) gain[1] = -FLT_MAX;
	if (!(existBox & 0b0001)) gain[2] = -FLT_MAX;
	switch (tmpDec) {//box_num 0,1,2
	case 0x00:
		switch (existBox) {//box_num 0,1,2
		case 0b0001:x = 2; break; case 0b0010:x = 1; break; case 0b0100:x = 0; break;
		default:
			if (gain[0] < gain[1]) {
				if (gain[1] < gain[2])x = 2;
				else x = 1;
			}
			else {
				if (gain[0] < gain[2])x = 2;
				else x = 0;
			}
			break;
		}
		break;
	case 0x08:
		switch (existBox) {//box_num 0,1,2
		case 0b0001:x = 2; break; case 0b0010:x = 1; break; case 0b0100:x = 0; break;
		case 0b0011:x = rnd() % 2 + 1; break;
		case 0b0101:x = rnd() % 2 << 1; break;
		case 0b0110:x = rnd() % 2; break;
		case 0b0111:
			x = rnd() % 3;
			break;
		}
		break;
	default:
		if (gain[0] < gain[1]) {
			if (gain[1] < gain[2])x = 2;
			else x = 1;
		}
		else {
			if (gain[0] < gain[2])x = 2;
			else x = 0;
		}
		break;
	}
	return x;
}

/*****************************************************************/
void study1() {//create all game state

	std::chrono::system_clock::time_point start, end;
	double elapsed = 0;
	double alltime = 0;

	char inFileName[32], outFileName[32];
	Boxes before;
	Boxes after;
	char num;
	char target;
	char turn;
	char fin;
	std::ifstream::pos_type pos_end;

	std::filesystem::create_directory("./turnBoxes");
	sprintf_s(outFileName, "./turnBoxes/turn0Boxes");
	std::ofstream output(outFileName, std::ios::binary);
	before.next_turn = 0;
	for (int i = 0; i < 8; ++i) {
		before.box[i] = 3;
	}
	before.box[3] = 0;
	before.box[7] = 0;
	output.write((char*)&before, sizeof(Boxes));
	output.close();

	for (int j = 1; j < 47; ++j) {
		start = std::chrono::system_clock::now();
		sprintf_s(inFileName, "./turnBoxes/turn%dBoxes", j - 1);
		std::ifstream input(inFileName, std::ios::binary);
		sprintf_s(outFileName, "./turnBoxes/turn%dBoxes", j);
		std::ofstream output(outFileName, std::ios::binary);
		input.seekg(0, std::ios_base::end);
		pos_end = input.tellg();
		input.seekg(0, std::ios_base::beg);
		while (input.tellg() != pos_end) {
			input.read((char*)&before, sizeof(char[9]));
			if ((before.next_turn & 0b1000))continue;
			else before.next_turn &= 0b0001;
			int i;
			for (i = 0; i < 3; ++i) {
				turn = before.next_turn << 2;
				target = turn + i;
				after = before;
				num = before.box[target];
				if (num == 0) continue;
				after.box[target] = 0;
				++target;
				for (int num2 = num; num2 > 0; --num2) {
					after.box[target] += 1;
					++target;
					if (target == 8)
						target = 0;
				}
				if ((target & 0b0011)) after.next_turn = (~after.next_turn) & 0b0001;
				if (after.box[turn] | after.box[turn + 1] | after.box[turn + 2])fin = 0b0000;
				else fin = 0b1000;
				after.next_turn = after.next_turn + (i << 1) + fin;
				output.write((char*)&after, sizeof(Boxes));
			}
		}
		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		alltime += elapsed;
		std::cout << j << ":" << elapsed << "microsec  /" << alltime << "microsec" << std::endl;
		input.close();
		output.close();
	}
}

/*
light tree vs heavy tree
heavy tree have number of winnable states
*/

void creTree() {//create state tree 
	std::chrono::system_clock::time_point start, end;
	double elapsed = 0;
	double alltime = 0;
	char beforeBoxesName[32], afterDataName[32], beforeDataName[32];
	Datum nDatum;
	Datum aDatum;
	int cnt, cnt_tmp;
	Boxes before;
	char turn;
	std::ifstream::pos_type pos_end;

	std::filesystem::create_directory("./treeLight");
	std::cout << "*************" << std::endl;
	for (int j = 46; j >= 0; --j) {
		start = std::chrono::system_clock::now();
		sprintf_s(beforeBoxesName, "./turnBoxes/turn%dBoxes", j);
		std::ifstream bBoxes(beforeBoxesName, std::ios::binary);
		sprintf_s(afterDataName, "./treeLight/tree%d", j + 1);
		std::ifstream aData(afterDataName, std::ios::binary);
		sprintf_s(beforeDataName, "./treeLight/tree%d", j);
		std::ofstream bData(beforeDataName, std::ios::binary);
		bBoxes.seekg(0, std::ios_base::end);
		pos_end = bBoxes.tellg();
		bBoxes.seekg(0, std::ios_base::beg);
		cnt = 0;
		cnt_tmp = 0;
		while (bBoxes.tellg() != pos_end) {
			bBoxes.read((char*)&before, sizeof(char[9]));
			if (before.next_turn & 0b1000) {
				if (before.box[0] | before.box[1] | before.box[2])nDatum.nextWin = 0b11101110;
				else nDatum.nextWin = 0b11100000;
			}
			else {
				turn = (before.next_turn & 0b0001) << 2;
				nDatum.nextWin = 0;
				for (int i = 0; i < 3; ++i) {
					if (before.box[turn + i]) {
						aData.read((char*)&aDatum, sizeof(char[5]));
						++cnt;
						switch (aDatum.nextWin & 0x0f) {
						case 0b1110: case 0b1111:
							nDatum.nextWin += 0b0001;
							break;
						case 0b0000: case 0b0001:
							nDatum.nextWin += 0b0000;
							break;
						default:
							nDatum.nextWin += (aDatum.nextWin & 0b0001);
						}
					}
					else {
						if (turn)nDatum.nextWin += 0b10000;
						else nDatum.nextWin += 0b10001;//non exist flag =1
					}
					nDatum.nextWin <<= 1;
				}
			}
			nDatum.nextWin += (before.next_turn & 0b0001);
			nDatum.loc = cnt_tmp;
			bData.write((char*)&nDatum, sizeof(char[5]));
			cnt_tmp = cnt;
		}
		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		alltime += elapsed;
		std::cout << j << ":" << elapsed << "microsec  /" << alltime << "microsec" << std::endl;
		bBoxes.close();
		aData.close();
		bData.close();

	}
}
void creTreeSt() {//create state tree 
	std::chrono::system_clock::time_point start, end;
	double elapsed = 0;
	double alltime = 0;
	char beforeBoxesName[32], afterDataName[32], beforeDataName[32];
	DatumExpansion nDatum;
	DatumExpansion aDatum;
	int cnt, cnt_tmp;
	Boxes before;
	char turn;
	std::ifstream::pos_type pos_end;

	std::filesystem::create_directory("./treeHeavy");
	std::cout << "*************" << std::endl;
	for (int j = 46; j >= 0; --j) {
		start = std::chrono::system_clock::now();
		sprintf_s(beforeBoxesName, "./turnBoxes/turn%dBoxes", j);
		std::ifstream bBoxes(beforeBoxesName, std::ios::binary);
		sprintf_s(afterDataName, "./treeHeavy/tree%d", j + 1);
		std::ifstream aData(afterDataName, std::ios::binary);
		sprintf_s(beforeDataName, "./treeHeavy/tree%d", j);
		std::ofstream bData(beforeDataName, std::ios::binary);
		bBoxes.seekg(0, std::ios_base::end);
		pos_end = bBoxes.tellg();
		bBoxes.seekg(0, std::ios_base::beg);
		cnt = 0;
		cnt_tmp = 0;
		while (bBoxes.tellg() != pos_end) {
			for (int k = 0; k < 6; ++k) nDatum.WinNum[k] = 0;
			bBoxes.read((char*)&before, sizeof(char[9]));
			if (before.next_turn & 0b1000) {
				if (before.box[0] | before.box[1] | before.box[2]) {
					nDatum.nextWin = 0b11101110;
					nDatum.WinNum[1] = 1;
				}
				else {
					nDatum.nextWin = 0b11100000;
					nDatum.WinNum[0] = 1;
				}
			}
			else {
				turn = (before.next_turn & 0b0001) << 2;
				nDatum.nextWin = 0;
				for (int i = 0; i < 3; ++i) {
					if (before.box[turn + i]) {
						aData.read((char*)&aDatum, sizeof(char[29]));
						++cnt;
						switch (aDatum.nextWin & 0x0f) {
						case 0b1110: case 0b1111:
							nDatum.nextWin += 0b0001;
							break;
						case 0b0000: case 0b0001:
							nDatum.nextWin += 0b0000;
							break;
						default:
							nDatum.nextWin += (aDatum.nextWin & 0b0001);
						}
						nDatum.WinNum[i << 1] = aDatum.WinNum[0] + aDatum.WinNum[2] + aDatum.WinNum[4];
						nDatum.WinNum[(i << 1) + 1] = aDatum.WinNum[1] + aDatum.WinNum[3] + aDatum.WinNum[5];
					}
					else {
						if (turn)nDatum.nextWin += 0b10000;
						else nDatum.nextWin += 0b10001;//non exist flag =1
					}
					nDatum.nextWin <<= 1;
				}
			}
			nDatum.nextWin += (before.next_turn & 0b0001);
			nDatum.loc = cnt_tmp;
			bData.write((char*)&nDatum, sizeof(char[29]));
			cnt_tmp = cnt;
		}
		end = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		alltime += elapsed;
		std::cout << j << ":" << elapsed << "microsec  /" << alltime << "microsec" << std::endl;
		bBoxes.close();
		aData.close();
		bData.close();

	}
}
