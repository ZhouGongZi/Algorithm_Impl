/*
We have a list of tasks and their durations
task_and_duration = {
	"A:1",
	"B:2",
	"C:3",
	"D:3",
	"E:3",
}

We have the task dependencies:
B:A means we need to finish A first in order to finish B
task_deps = {
	"B:A",
	"C:A",
	"D:A,B",
	"E:B,C",
}

Given the above two inputs, determine the order of tasks to be finished.
The result should be return as
Duration - list of task being done in the duration

Note, one task can be split into multiple durations, for example:
the next 2 mins, we are doing B and C, but since C needs 3 mins, it will
be continue in the 1 min after the 2 mins duration.

expected_output = {
	"1, A",
	"2, B, C",
	"1, C, D",
	"2, D, E",
	"1, E"		
}
*/
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;


/*
debug helper
*/
void printInv(unordered_map<char, vector<char>>& inv) {
	for (auto it = inv.begin(); it != inv.end(); ++it) {
		cout << it->first << " has deps: ";
		for (char c : it->second) cout << c << ", ";
		cout << endl;
	}	
}

void printDegree(unordered_map<char, int>& indegree) {
	for (auto it = indegree.begin(); it != indegree.end(); ++it) {
		cout << it->first << " indegree:  " << it->second << endl;
	}
}

void printDur(unordered_map<char, int>& task_duration) {
	for (auto it = task_duration.begin(); it != task_duration.end(); ++it) {
		cout << it->first << " duration:  " << it->second << endl;
	}
}

/* functions */
vector<string> getTaskSchedule(unordered_map<char, int>& task_duration,
							   unordered_map<char, vector<char>>& deps) {
	vector<string> res;
	int n = task_duration.size();

	// Process deps for: 1. in-degree, 2. Invert index for deps
	unordered_map<char, int> indegree;
	unordered_map<char, vector<char>> inv;
	for (auto it = deps.begin(); it != deps.end(); ++it) {
		indegree[it->first] += it->second.size();
		for (char c : it->second)
			inv[c].push_back(it->first);
	}
	for (auto it = task_duration.begin(); it != task_duration.end(); ++it) {
		if (!deps.count(it->first)) indegree[it->first] = 0;
	}

	while (1) {
		// search for all the nodes that has indegree 0
		// get min duration for task in the result string in this round.
		vector<char> outer;
		int minDur = INT_MAX;
		for (auto it = indegree.begin(); it != indegree.end(); ++it) {
			if (it->second == 0) {
				outer.push_back(it->first);
				minDur = min(minDur, task_duration[it->first]);
			}
		}
		
		// Need to verify/change the checking a little if there is cycle involved.
		if (outer.empty()) return break;

		// For all task in outer, update their duration
		// 1. if it becomes zero, update indegree as -1 (task done, put in string)
		// 2. if it is not zero, let it be, it will be found in next round
		string resStr = to_string(minDur);
		resStr.push_back(',');
		for (char c : outer) {
			resStr.push_back(c);
			resStr.push_back(',');
			if (task_duration[c] == minDur) {
				indegree[c] = -1;
				for (char d: inv[c]) indegree[d]--;
			}
			task_duration[c] -= minDur;
		}
		res.push_back(resStr);
	}
	return res;
}

int main () {
	unordered_map<char, int> task_duration;
	task_duration['A'] = 1;
	task_duration['B'] = 2;
	task_duration['C'] = 3;
	task_duration['D'] = 3;
	task_duration['E'] = 3;

	unordered_map<char, vector<char>> deps;
	deps['B'] = {'A'};
	deps['C'] = {'A'};
	deps['D'] = {'A', 'B'};
	deps['E'] = {'B', 'C'};

	vector<string> res = getTaskSchedule(task_duration, deps);
	cout << "res size " << res.size() << endl;
	for (auto& s : res) {
		cout << s << endl;
	}

	return 0;
}





