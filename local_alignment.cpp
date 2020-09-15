#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <algorithm>
using namespace std;

struct table_cell {
    int score;
    int prev_row;
    int prev_col;
};
double local_alignment(string s1, string s2, int match, int mismatch, int gap);
void trace_back(string s1, string s2, vector<vector<table_cell>> dp_table, int max_row, int max_col);

int main() {
    string s1, s2;
    s1 = "GGTTGACTA";
    s2 = "TGTTACGG";
    int match = 1;
    int mismatch = -1;
    int gap = -1;

    local_alignment(s1, s2, match, mismatch, gap);
    return 0;
}


double local_alignment(string s1, string s2, int match, int mismatch, int gap) {

    int m = s2.length()+1;
    int n = s1.length()+1;
    vector<vector<table_cell>> dp_table;
    dp_table.resize(m, vector<table_cell>(n));
    int i,j,max_row, max_col;
    int max_score=0, insert_score, del_score, sub_score, cell_score;
    // initialize dp_table
    for(i=0; i<m; i++) {
        dp_table[i][0].score = 0;
        dp_table[i][0].prev_row = -1;
        dp_table[i][0].prev_col = -1;
    }
    for(j=0; j<n; j++) {
        dp_table[0][j].score = 0;
        dp_table[0][j].prev_row = -1;
        dp_table[0][j].prev_col = -1;
    }
    for(i=1; i<m; i++) {
        for(j=1; j<n; j++) {
            insert_score = dp_table[i-1][j].score+gap;
            del_score = dp_table[i][j-1].score+gap;
            //sub_score = dp_table[i-1][j-1].score + (s2[i-1]==s1[j-1]) ? match : mismatch;
            if(s2[i-1]==s1[j-1]) {
                sub_score = dp_table[i-1][j-1].score + match;
            }else{
                sub_score = dp_table[i-1][j-1].score + mismatch;
            }
            cell_score = max(max(insert_score, del_score), sub_score);
            if(cell_score < 0) {
                dp_table[i][j].score = 0;
                dp_table[i][j].prev_row = -1;
                dp_table[i][j].prev_col = -1;

            }else if(cell_score==sub_score) {
                dp_table[i][j].score = cell_score;
                dp_table[i][j].prev_row = i-1;
                dp_table[i][j].prev_col = j-1;
            }else if(cell_score==insert_score) {
                dp_table[i][j].score = cell_score;
                dp_table[i][j].prev_row = i-1;
                dp_table[i][j].prev_col = j;

            } else if(cell_score==del_score) {
                dp_table[i][j].score = cell_score;
                dp_table[i][j].prev_row = i;
                dp_table[i][j].prev_col = j-1;
            }
            if(dp_table[i][j].score > max_score) {
                max_score = dp_table[i][j].score;
                max_row = i;
                max_col = j;
            }
        }
    }
    //print dp_table
    for(i=0; i<m; i++) {
        for(j=0; j<n; j++) {
            cout << dp_table[i][j].score << " ";
        }
        cout << endl;
    }

    trace_back(s1, s2, dp_table, max_row, max_col);
    return max_score;
}

void trace_back(string s1, string s2, vector<vector<table_cell>> dp_table, int max_row, int max_col) {
    int i=max_row;
    int j=max_col;
    int prev_i, prev_j;
    string output_s1,output_s2;

    while(i>0 && j>0) {
        if(dp_table[i][j].score==0) break;

        prev_i = dp_table[i][j].prev_row;
        prev_j = dp_table[i][j].prev_col;
        if((prev_i<0) || (prev_j<0)) {break;}
        else if((prev_i==(i-1)) && (prev_j==(j-1))) { // came diagonally, so it would be a substitution
            output_s1 += s1[j-1];
            output_s2 += s2[i-1];
        } else if((prev_i==(i-1)) && (prev_j==j)) { // insert a gap in s1
            output_s1 += '-';
            output_s2 += s2[j-1];
        } else { // insert a gap in s2
            output_s1 += s1[j-1];
            output_s2 += '-';
        }
        i = prev_i;
        j = prev_j;
    }

    cout << "Original sequences: " << endl;
    cout << s1 << endl;
    cout << s2 << endl;
    cout << "Aligned sequences: " << endl;
    reverse(output_s1.begin(), output_s1.end());
    reverse(output_s2.begin(), output_s2.end());
    cout << output_s1 << endl;
    cout << output_s2 << endl;
}
