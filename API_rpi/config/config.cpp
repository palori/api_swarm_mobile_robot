#include "config.h"

void read_file(string fileName){
	printf("\n\nTesting 'read_file':\n");

	ifstream file;

	file.open(fileName);
	string text = "";
	cout << "\n\nFile: " << file.get() << "\n\n";
	if (file.is_open()) {

		// get length of file:
    	file.seekg (0, file.end);
    	int len = file.tellg();
    	file.seekg (0, file.beg);

    	// get full text in a string
		char c[len];
		file.read(c, len);
		for (int i = 0; i < len; i++){ // from char array to string
			text += c[i];
		}
		
	}
	file.close();

	cout << "Text: \n" << text << "\n\n";

	vector<string> rows = split_str(text, "\n");

	/*for (int i = 0; i < rows.size(); i++){
		cout << "row " << i << ": " << rows.at(i) << endl;
	}*/

	vector<string> headers = split_str(rows.at(0), ", ");
	/*cout << "\n\nheaders:\n";
	for (int i = 0; i < headers.size(); i++){
		if (headers.at(i) == ""){
			headers.erase(headers.begin()+i);
			i--; // I don't know if this is doing something, but it's better to keep it :)
		}
		else cout << headers.at(i) << endl;
	}*/


	for (int i = 1; i < rows.size(); i++){
		vector<string> row_data = split_str(rows.at(i), ", ");

		// Do something based on the name of the file
		// like saving the data in some specific classes created to store the config.
		// - map info (nodes, edges)
		// - list of different task types the robots should do (# robots needed...)
		// - list of tasks to do in some of the nodes of the map

	}

}