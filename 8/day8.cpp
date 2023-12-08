#include <iostream>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <regex>
#include <thread>
#include <future>

using namespace std;

vector<string> tokenizeString(string s, char delim) {
  vector<string>tokens;
  stringstream sline(s);
  string temp;
  while(getline(sline, temp, delim)) {
    tokens.push_back(temp);
  }
  return tokens;
}

long followInstructions(string node, const string& instructions, map<string, pair<string, string>>& connections) {
  int steps = 0;
  string currentNode = node;
  for(int i = 0; i < instructions.size(); i++) {
    steps++;
    if (currentNode[2] == 'Z') {
      break;
    }
    currentNode = (instructions[i] == 'L') ? connections[currentNode].first : connections[currentNode].second;
    if(i == instructions.size() - 1) {
      i = -1;
    }
  }
  return --steps;
}

long LcmOfArray(vector<int> arr, int idx){
    if (idx == arr.size()-1){
        return arr[idx];
    }
    long a = arr[idx];
    long b = LcmOfArray(arr, idx+1);
    return (a*b/__gcd(a,b)); // __gcd(a,b) is inbuilt library function
}

long returnSteps(ifstream* input) {
  vector<string> currentNodes;
  string line;
  string instructions;
  map<string, pair<string, string>> connections;

  while(getline(*input, line)) {
     if (line.size() != 16 && line[0] != '\n') {
        // This is the instructions line
        instructions.append(line);
      } else if (line[0] == '\n') {
        continue;
      } else {
        vector<string> tokens = tokenizeString(line, '=');
        string nodeName = tokenizeString(tokens[0], ' ')[0];
        if (nodeName[2] == 'A') {
          currentNodes.push_back(nodeName);
        }
        vector<string> connectionTokens = tokenizeString(tokens[1], ',');
        string leftNode = regex_replace(tokenizeString(connectionTokens[0], ' ')[1], regex("\\(|\\)"), "");
        string rightNode = regex_replace(tokenizeString(connectionTokens[1], ' ')[1], regex("\\(|\\)"), "");
        connections[nodeName] = make_pair(leftNode, rightNode);
      }
  }
  //cout << "Instruction length " << instructions.size() << "\n";

  int nthreads = currentNodes.size();
  future<long> thread_arr[nthreads];
  for(int i = 0; i < nthreads; i++) {
    thread_arr[i] = async(followInstructions, currentNodes[i], instructions, ref(connections));
  }
  vector<int> steps;
  for(int i = 0; i < nthreads; i++) {
    steps.push_back(thread_arr[i].get());
  }
  return LcmOfArray(steps, 0);

}

int main (int argc, char *argv[])
{
  if (argc != 2) {
    perror("not enough args");
    exit(1);
  }
  std::ifstream input (argv[1]);
  int sum = 0;
  if (input.is_open()) {
    cout << "Number of steps: " << returnSteps(&input) << "\n";
  }
  return 0;
}
