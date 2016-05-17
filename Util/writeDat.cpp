#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define ONE_ITEM_PER_TRANS

main(int argc, char *argv[])
{	char *ifname, *ofname;

	if (argc < 3) {
		cerr << "usage: " << argv[0] << " input output\n";
		return -1;
	}
	ifname = argv[1]; ofname = argv[2];
	
	ifstream in(ifname);
	ofstream out(ofname);
	
	int item, itemx;
	static int END_OF_TRANS = -1;
	static int END_OF_CUST = -2;

	if (!in){
		cerr << "failed to open input file: " << ifname << endl;
		return -1;
	}
	if (!out){
		cerr << "failed to open output file: " << ofname << endl;
		return -1;
	}

	/* input format: 
	 *   item1 item2 ...
	 *   assume one line for one customer, and only one trans per customer
	 *   or 
	 *   assume one line for one customer, and only one item per trans
	 */
	string line;
	while (!getline(in, line).eof()){
		stringstream ss(line);
		
		item = -1;
		while (ss >> item){
			//if (item == itemx)
			//	continue;
			out.write((char*)(&item), 4);
#ifdef ONE_ITEM_PER_TRANS
			out.write((char*)&END_OF_TRANS, 4);
			itemx = item;
#endif
		}

#ifndef ONE_ITEM_PER_TRANS
		out.write((char*)&END_OF_TRANS, 4);		
#endif
		out.write((char*)&END_OF_CUST, 4);		
	}
	out.close();
	in.close();
}

