/*
*  test search functions; search wtih recursive method and 
*  with non recursive breadth first
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


using std::cout;
using std::endl;

using namespace kptree;
using namespace recursive_search_insert;


int main(int argc, char **argv)
{

	constexpr auto N = 26;
	char frChars [N] = {};
	tree<char> largeTree;
	auto status { false};

	// test chars with some random chars
	for (auto i =0; i < N ; i++)
	{
		frChars[i] = 'a'+i;
	}

	for (auto i =0; i < 6 ; i++)
	{
		largeTree.insert(largeTree.begin(), frChars[i]);
	}

	std::srand(0); // use current time as seed for random generator	
	for(tree<char>::sibling_iterator iRoots = largeTree.begin(); iRoots != largeTree.end(); ++iRoots)
	{
		auto random_variable = std::rand();
		auto id = std::rand()/((RAND_MAX + 1u)/N);
		auto id_left = id;

		auto id_right = (id + 5) % N;
		auto id3 = (id + 9) % N;
		auto level2 = largeTree.append_child(iRoots,frChars[id_left]);
		largeTree.append_child(iRoots,frChars[id_right]);
		largeTree.append_child(level2,frChars[id3]);
	}

	LOG_INFO(_log) <<"tree with depth 3"<< endl;
	print_tree(largeTree, largeTree.begin(), largeTree.end());

	char input_2[] = "ac";
	status = search_word_recursive(largeTree,2,input_2);
	cout << "found " << input_2 << ":" << status << endl;



	char input_2_f[] = "sx";
	status = search_word_recursive(largeTree,2,input_2_f);
	LOG_INFO(_log) << "found " << input_2_f << ":" << status << endl;

	char input_3[] = "sxg";
	status = search_word_recursive(largeTree,3,input_3);
	LOG_INFO(_log) << "found " << input_3 << ":" << status << endl;

	char input_not_found[] = "sxh";
	status = search_word_recursive(largeTree,3,input_not_found);
	LOG_INFO(_log) << "found " << input_not_found << ":" << status << endl;



	char input_4[] = "sxgv";

	status = search_word_recursive(largeTree,4,input_4);
	LOG_INFO(_log) << "found " << input_4 << ":" << status << endl;
	char input_5[] = "box";

	status = search_word_depth(largeTree,3,input_5);
	
	char input_6[] = "boxv";

	status = search_word_depth(largeTree,4,input_6);
	return 0;
}

