#include <iostream>
#include <fstream>

using namespace std;

/* format: item item -1(end of trans) item item -1(end of trans) 
 *         -2(end of trans seq by one customer 
 * output format is what writeDat.cpp want for its input
 */
main(int argc, char *argv[])
{	char *fname;
	int item;
	
	if (argc == 2) fname = argv[1];
	else {
		cerr << "usage: " << argv[0] << " input\n";
		return -1;
	}
	
	ifstream in(fname);
	if (!in) return -1;

	while (!in.read((char*)&item, sizeof(item)).fail()){
		if (item == -2) continue; /* next customer */
		
		if (item >= 0) cout << item << ' ';
		else if (item == -1) cout << '\n';
		else {
			cout << "file end prematurely\n";
			break;
		}
	}
	in.close();
}

