
for (int i = 0; i<960; i++){
	cout << endl << "[" << i << "] ";
	for (int j = 0; j<1280; j++){
		if (bin.at<uchar>(i,j)) cout << bin.at<uchar>(i,j); 
		else cout << " ";
	}
}

int left_edge[960];
int right_edge[960];
bool left_edge_found = false;
bool line_detected[960];
//bool right_edge_found = false;

for (int i = 0; i<960; i++){
	//cout << endl << "[" << i << "] ";
	left_edge[i] = 0;
	right_edge[i] = 1279;

	for (int j = 0; j<1280; j++){
		if (bin.at<uchar>(i,j) == 1){
			right_edge[i] = j;
			if (!left_edge_found) {
				left_edge[i]=j;
				left_edge_found=true;
			}
		} 
	}
	
	if (left_edge[i]!=0 && right_edge[i]!=1279) line_detected[i] = true;
	else line_detected[i] = false;
}

