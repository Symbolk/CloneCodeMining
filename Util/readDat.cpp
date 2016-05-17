#include <iostream>
#include <fstream>

using namespace std;

/* format: item item -1(end of trans) item item -1(end of trans) 
 *         -2(end of trans seq by one customer 
 */
main(int argc, char *argv[])
{	char *fname;
	int item;
	int cid, tid;
	
	if (argc == 2) fname = argv[1];
	else {
		cerr << "Usage: " << argv[0] << " input_file\n";
		return -1;
	}
		
	ifstream in(fname);
	if (!in) {
		cerr << "failed to open file: " << fname << '\n';
		return -1;
	}

	cid = 1;
	while (!in.read((char*)&item, sizeof(item)).fail()){
		if (item == -2){
			cid++;
			continue;
		}
		
		cout << "CID: " << cid << " TID: " << tid << " (";
		if (item >= 0){
			do {	
				cout << ' ' << item;
			} while (!in.read((char*)&item, sizeof(item)).fail() && item != -1);
			/* one transaction */
		}
		if (item == -1){
			tid ++;
			cout << ")\n";
		} else {
			cout << "file end prematurely\n";
			break;
		}
	}
done:
	in.close();
}

