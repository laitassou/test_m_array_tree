/*
* Test search with substitute 
*
*/
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <list>
#include <utility>
#include <stdexcept>
#include <ctime>

#include "tree.hh"
#include "tree_util.hh"
#include "recursive_search.hpp"

using namespace kptree;
using namespace recursive_search_insert;

int main(int argc, char **argv)
{
	constexpr auto N = 26;
	char frChars [N] = {};
	tree<char> largeTree;
	tree<char> result;

	// test chars
	for (auto i =0; i < N ; i++)
	{
		frChars[i] = 'a'+i;
	}

	for (auto i =0; i < 3 ; i++)
	{
		largeTree.insert(largeTree.begin(), frChars[i]);
	}

	std::srand(0); // use current time as seed for random generator	
	for(tree<char>::sibling_iterator iRoots = largeTree.begin(); iRoots != largeTree.end(); ++iRoots)
	{
		auto id = std::rand()/((RAND_MAX + 1u)/N);
		for(auto i =1; i <3;i++)
		{
			auto level = largeTree.append_child(iRoots, frChars[(id+i) % N]);
			for(auto j =1; j <2; j++)
			{
				largeTree.append_child(level, frChars[(id+j) % N]);
			}
		}
	}
	LOG_DEBUG(_log) <<" 1 level "<<endl;
	//print_tree_bracketed(largeTree);
	print_tree(largeTree, largeTree.begin(), largeTree.end());


	char input_3[] = "bal";
	LOG_INFO(_log) <<"  search word: "<<input_3 <<endl;

	tree_search_substitute(largeTree,3,input_3,1,result);

	return 0;
}

