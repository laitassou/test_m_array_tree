/* 

	STL-like templated tree class; test program.
	Copyright (C) 2001-2009  Kasper Peeters <kasper.peeters@aei.mpg.de>

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
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


void print_tree(const tree<char>& tr, tree<char>::pre_order_iterator it, tree<char>::pre_order_iterator end)
{
	if(!tr.is_valid(it)) return;
	int rootdepth=tr.depth(it);
	std::cout << "-----" << std::endl;
	while(it!=end) {
		for(auto i=0; i<tr.depth(it)-rootdepth; ++i) 
			std::cout << "  ";
		std::cout << (*it) << std::endl << std::flush;
		++it;
		}
	std::cout << "-----" << std::endl;
}

// decl
void test1();
void test2();
void test3();
void test4();
void test5();


int main(int argc, char **argv)
{

	test1();

	test2();

	test3();

	test4();

	test5();

	return 0;
}


void test1()
{

	constexpr auto N = 26;
	constexpr auto DEPTH = 3;
	constexpr auto WIDTH = 3;

	char frChars [N] = {};
	tree<char> _tree;
	tree<char> result;
	auto count = 0;

	// test chars with some random chars
	for (auto i =0; i < N ; i++)
	{
		frChars[i] = 'a'+i;
	}

	for (auto i =0; i < WIDTH ; i++)
	{
		_tree.insert(_tree.begin(), frChars[i]);
		count++;
	}

	std::srand(0); // use current time as seed for random generator	
	auto status = false;
	typename tree<char>::pre_order_iterator it_depth;
	auto current_depth = 0;
	auto count_depth = 0;
	
	std::vector<typename tree<char>::iterator> vec;


	typename tree<char>::iterator _begin= _tree.begin();
	typename tree<char>::iterator _end= _tree.end();

	it_depth = _begin;

	for (auto i = 0; i < DEPTH; i++)
	{

		it_depth = _begin;
		while(it_depth != _end)
		{
			current_depth = _tree.depth(it_depth);			
			LOG_INFO(_log) << *it_depth <<", depth :"<<current_depth <<",i:" <<i<<endl;

			//if (_tree.number_of_children(it_depth) == 0)
			{
				vec.push_back(it_depth);
			}

			it_depth++;
		}

		for(auto it =vec.begin(); it != vec.end(); it++)
		{
			auto id = std::rand()/((RAND_MAX + 1u)/N);
			{
				auto level = _tree.append_child(*it, frChars[(id) % N]);
				count++;
			}	
		}		
		vec.empty();

	LOG_INFO(_log) <<  "count:" <<count <<endl;
	}


	LOG_INFO(_log) <<"word  bxv "<<endl;
	//print_tree_bracketed(largeTree);
	print_tree(_tree, _tree.begin(), _tree.end());

	char input_2[] = "xv";
	tree_search_add_erase(_tree,2,input_2,1,1,1,result);	
}

void test2()
{
	constexpr auto N = 26;
	char frChars [N] = {};
	tree<char> _tree;
	tree<char> result;

	// test chars
	for (auto i =0; i < N ; i++)
	{
		frChars[i] = 'a'+i;
	}

	for (auto i =0; i < 3 ; i++)
	{
		_tree.insert(_tree.begin(), frChars[i]);
	}


	std::srand(0); // use current time as seed for random generator
	
	for(tree<char>::sibling_iterator iRoots = _tree.begin(); iRoots != _tree.end(); ++iRoots)
	{
		auto id = std::rand()/((RAND_MAX + 1u)/N);
		for(auto i =1; i <5;i++)
		{
			auto level = _tree.append_child(iRoots, frChars[(id+i) % N]);
			for(auto j =1; j <5; j++)
			{
				level = _tree.append_child(level, frChars[(id+j) % N]);
			}
		}
	}
	LOG_DEBUG(_log) <<" 1 level "<<endl;
	//print_tree_bracketed(largeTree);
	print_tree(_tree, _tree.begin(), _tree.end());


	char input[] = "bx";
	LOG_INFO(_log) <<"  search word: "<<input <<endl;

		tree_search_add_erase(_tree,2,input,1,1,1,result);
}


void test3()
{
	constexpr auto N = 26;
	char frChars [N] = {};
	tree<char> _tree;
	tree<char> result;
	constexpr auto DEPTH = 3;
	constexpr auto WIDTH = 3;

	// test chars
	for (auto i =0; i < N ; i++)
	{
		frChars[i] = 'a'+i;
	}

	for (auto i =0; i <4; i++)
	{
		_tree.insert(_tree.begin(), frChars[i]);
	}


	std::srand(0); // use current time as seed for random generator
	
	for(tree<char>::sibling_iterator iRoots = _tree.begin(); iRoots != _tree.end(); ++iRoots)
	{
		auto id = std::rand()/((RAND_MAX + 1u)/N);
		auto u = 2+ std::rand()/((RAND_MAX + 1u)/WIDTH);
		auto v = 1 + std::rand()/((RAND_MAX + 1u)/DEPTH);

		for(auto i =1; i <u;i++)
		{
			auto level = _tree.append_child(iRoots, frChars[(id+i) % N]);
			for(auto j =1; j <v; j++)
			{
				level = _tree.append_child(level, frChars[(id+j) % N]);
			}
		}
	}
	LOG_DEBUG(_log) <<" 1 level "<<endl;
	//print_tree_bracketed(largeTree);
	print_tree(_tree, _tree.begin(), _tree.end());


	char input[] = "bxzw";
	LOG_INFO(_log) <<"  search word: "<<input <<endl;

		tree_search_add_erase(_tree,4,input,1,1,1,result);
}



void test4()
{

	constexpr auto N = 26;
	constexpr auto DEPTH = 7;
	constexpr auto WIDTH = 6;

	char frChars [N] = {};
	tree<char> _tree;
	tree<char> result;
	auto count = 0;

	// test chars with some random chars
	for (auto i =0; i < N ; i++)
	{
		frChars[i] = 'a'+i;
	}

	for (auto i =0; i < WIDTH ; i++)
	{
		_tree.insert(_tree.begin(), frChars[i]);
		count++;
	}

	std::srand(0); // use current time as seed for random generator	
	auto status = false;
	typename tree<char>::pre_order_iterator it_depth;
	auto current_depth = 0;
	auto count_depth = 0;
	
	std::vector<typename tree<char>::iterator> vec;


	typename tree<char>::iterator _begin= _tree.begin();
	typename tree<char>::iterator _end= _tree.end();

	it_depth = _begin;

	for (auto i = 0; i < DEPTH; i++)
	{

		it_depth = _begin;
		while(it_depth != _end)
		{
			current_depth = _tree.depth(it_depth);			
			LOG_INFO(_log) << *it_depth <<", depth :"<<current_depth <<",i:" <<i<<endl;


			//if (_tree.number_of_children(it_depth) == 0)
			{
				vec.push_back(it_depth);
			}

			it_depth++;
		}

		for(auto it =vec.begin(); it != vec.end(); it++)
		{
			auto id = std::rand()/((RAND_MAX + 1u)/N);
			{
				auto level = _tree.append_child(*it, frChars[(id) % N]);
				count++;
			}	
		}		
		vec.empty();

	LOG_INFO(_log) <<  "count:" <<count <<endl;
	}


	LOG_INFO(_log) <<"word  bxv "<<endl;
	//print_tree_bracketed(largeTree);
	print_tree(_tree, _tree.begin(), _tree.end());
/*

	char input_3[] = "bxv";
	tree_search_add_erase(_tree,3,input_3,1,1,1,result);

	LOG_INFO(_log) <<endl << " word xv "<<endl;
*/
	char input_2[] = "modert";
	tree_search_add_erase(_tree,6,input_2,1,1,1,result);	
}



// example of search where match is full 
void test5 ()
{


	constexpr auto N = 26;
	char frChars [N] = {};
	tree<char> _tree;
	tree<char> result;

	// test chars
	for (auto i =0; i < N ; i++)
	{
		frChars[i] = 'a'+i;
	}

	for (auto i =0; i < 3 ; i++)
	{
		_tree.insert(_tree.begin(), frChars[i]);
	}


	std::srand(0); // use current time as seed for random generator
	
	for(tree<char>::sibling_iterator iRoots = _tree.begin(); iRoots != _tree.end(); ++iRoots)
	{
		auto id = 0;
		for(auto i =1; i <3;i++)
		{
			auto level = _tree.append_child(iRoots, frChars[(id+i) % N]);
			id++;
			for(auto j =1; j <2; j++)
			{
				_tree.append_child(level, frChars[(id+j) % N]);
				id++;
			}
		}
	}
	LOG_DEBUG(_log) <<" 1 level "<<endl;
	//print_tree_bracketed(largeTree);
	print_tree(_tree, _tree.begin(), _tree.end());


	char input_3[] = "bee";
	LOG_INFO(_log) <<"  search word: "<<input_3 <<endl;

	tree_search_add_erase(_tree,3,input_3,1,1,1,result);	

}
