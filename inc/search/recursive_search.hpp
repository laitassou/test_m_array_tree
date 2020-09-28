
/* recursive search
 * limitation : stack  size, depth of tree
 */

#ifndef _RECURSIVE_SEARCH_
#define _RECURSIVE_SEARCH_

#include <iostream>
#include <string>

#include <stack>
#include <map>

#include <vector>

#include "cpplog.hpp"

using namespace cpplog;
using namespace std;

using std::cout;
using std::endl;

namespace recursive_search_insert {


StdErrLogger _log;


#define MIN(x,y) (x < y) ? x:y;
		
// decl
template<class T>
bool subtree_search(const tree<T>& t, typename tree<T>::iterator iRoot, int n, T a[]);

/*
 * recursive version of search
 */
template <class T> bool search_word_recursive(const tree<T>& t, int n, T a[])
{

	auto found = false;
	for(typename tree<T>::sibling_iterator iRoots = t.begin(); iRoots != t.end(); ++iRoots)
	{
		if(*iRoots == a[0])
		{
			LOG_INFO(_log)  <<"root found :" << n << "," <<t.number_of_children(iRoots) << "," <<*iRoots<< endl;
			if (n == 1 && t.number_of_children(iRoots) == 0)
			{
				//str << *iRoot;
				found = true;	
			}
			else 
			{
				found = subtree_search(t, iRoots, n-1, a+1);
			}

			if(found)
			{
				break;
			}
		}
	}
	return found;
}


template<class T>
bool subtree_search(const tree<T>& t, typename tree<T>::iterator iRoot, int n, T a[])
{
	auto status = false;
	LOG_DEBUG(_log)  << "root:" <<*iRoot<< ",a0: " << a[0] <<",n:" <<n<< endl;

	// child1, ..., childn
	typename tree<T>::sibling_iterator iChildren;
	for (iChildren = t.begin(iRoot); iChildren != t.end(iRoot); ++iChildren)
	{
		LOG_DEBUG(_log) <<"child :"<< *iChildren << endl;
		if(*iChildren == a[0])
		{
			if(t.number_of_children(iChildren) == 0)
			{
				// match exactly word
				if(n == 1)
				{
					status = true;
				}
				// break anyway
				break;
			}
			else
			{ 
				status = subtree_search(t,iChildren,n-1, a+1);
				if(status)
				{ 
					break;
				}
			}
		}
	}
	return status;

}

/*
* non recursive search, search using depth first iterator  
*/
template <class T> bool search_word_depth(const tree<T>& t, int word_length, T word[])
{
	auto status {false};

	typename tree<T>::sibling_iterator current_root, root = t.begin(); 

	auto current_depth = 0;

	while (root != t.end())
	{
		if( status == true)
		{
			break;
		}

		current_depth = t.depth(root);
		// go no next element in same level [sibling iterator] if current one doesn't match
		if(*root != word[current_depth]) 
		{
			root++;
			continue;
		}

		// if it matchs then go in next depth
		current_root = t.begin(root);
		while(current_root != t.end(root)) 
		{
			current_depth = t.depth(current_root);
			if(*current_root !=  word[current_depth] )
			{
				current_root++;
				continue;
			}

			LOG_DEBUG(_log)  <<"*current_root" <<*current_root << ",current_depth" << current_depth << endl;

			// reached end of branch
			if(t.number_of_children(current_root) == 0  && (current_depth+1) ==word_length )
			{
				status = true;
				break;
			}
			current_root = t.begin(current_root);			
		}
		root++;
	}

	if(status == true)
	{
		LOG_INFO(_log)  <<"word " <<word <<" found" << endl;
	}
	else
	{
		LOG_INFO(_log)  <<"word " <<word <<" not found" << endl;
	}
	return status;

}

/*
* insert to tree
**/
template <class T>
void insert_word(tree<T>& t, int n, T input_word[])
{
	auto index = 0;
	auto status = false;
	auto foundRoot = false;
	auto foundEndPattern = false;
	typename tree<T>::iterator matchRoot;

	for(typename tree<T>::sibling_iterator iRoots = t.begin(); iRoots != t.end(); ++iRoots)
	{
		if(*iRoots == input_word[0])
		{
			foundRoot = true;

			matchRoot = iRoots;
			index++;
			LOG_DEBUG(_log) << "root :" <<*iRoots << endl;
			do {
				LOG_INFO(_log) << "matchRoot :" <<*matchRoot << endl;

				typename tree<T>::sibling_iterator iChildren;

				if (t.number_of_children(matchRoot) == 0)
				{	
					foundEndPattern = true;
				}
				else
				{
					for (iChildren = t.begin(matchRoot); iChildren != t.end(matchRoot); ++iChildren)
					{
						LOG_DEBUG(_log) <<"child :"<<  *iChildren << endl;
						if(*iChildren == input_word[index])
						{	
							LOG_DEBUG(_log) <<"child :"<<  *iChildren << "index:"<<index <<"a:"<<input_word[index]<< endl;
							index++;
							matchRoot = iChildren;
							break;
						}
					
					}
				
					if(iChildren == t.end(matchRoot))
						foundEndPattern = true;

					LOG_DEBUG(_log) << "matchRoot :" <<*matchRoot <<",index:" << index <<","<< ",foundEndPattern:" <<foundEndPattern << endl;
				}
			}while(foundEndPattern == false);

			break;
		}
	}
	if(foundRoot == true)
	{
		//auto level =matchRoot;
		// apppend to parent why = > avoid hiding existing words
		// example 
		// we have this word b c d and we would insert bcdef
		// we should obtain this result : char  d is repeated and it must be
		//	b
		//		c
		//			d
		//			d
		//				e
		//					f

		// last char that matchs at end, so add remaining to previous level
		auto i = 0;
		if(t.number_of_children(matchRoot) == 0)
		{
			auto level = t.parent(matchRoot);
			LOG_DEBUG(_log) <<"match "<<*matchRoot <<"," << *level << "," <<index <<endl;
			i = index -1 ;
			for(auto i = index-1 ; i < n; i++)
			{
				level = t.append_child(level, input_word[i]);
			}	

		}
		else  // last char that matchs before , so don't duplicate this char
		{
			auto level = (matchRoot);			
			LOG_DEBUG(_log) <<"match "<<*matchRoot <<"," << *level << "," <<index <<endl;
			for(auto i = index ; i < n; i++)
			{
				level = t.append_child(level, input_word[i]);
			}	
		}		
	}
	else // No common root found
	{
		auto level = t.begin();
		level = t.insert(level, input_word[0]);
		for(auto i = 1 ; i < n; i++)
		{
			level = t.append_child(level, input_word[i]);
		}		
	}

}

/*
* this means search all words that match at least n-nb_allowed_substitute characters
* precondition : word length must be equal to n
* one traversal of tree, traversal in depth
* To do stop when depth of branch > n and go to next branch
*/
template<class T>
bool tree_search_substitute(const tree<T>& t, int n, T target_word[], int nb_allowed_substitute, tree<T>& tfound)
{
	auto status = false;
	auto index = 0;
	std::vector<int> match;
	std::stack<T> _stack;

	typename tree<T>::pre_order_iterator it_depth;
	bool end_reached {false};
	auto current_depth = 0, last_depth = 0;

	auto need_to_match = n - nb_allowed_substitute;

	if(nb_allowed_substitute >= n)
	{
		LOG_WARN(_log) << "Number of allowed substitute:"<< nb_allowed_substitute << " is higher than n :"<<n <<endl;
		return false;
	}

	it_depth = t.begin();
	while(it_depth != t.end())
	{
			current_depth = t.depth(it_depth);
			if(end_reached)
			{
				LOG_DEBUG(_log) << "stack :"<< _stack.size()<< ",match:" << match.size()<< ",cur:"<<current_depth << ",last:"<<last_depth <<endl;
				for(auto u= current_depth; u <= last_depth; u++)
				{
					_stack.pop();
					match.pop_back();
				}
				end_reached = false;
			}
			
			LOG_DEBUG(_log) << *it_depth <<", depth :"<<t.depth(it_depth) << ",index:" <<index<<endl;
			if(*it_depth == target_word[t.depth(it_depth)])
			match.push_back(1);
			else
			match.push_back(0);


			_stack.push(*it_depth);

			if(t.number_of_children(it_depth) == 0)
			{
				auto sum_match = 0;
				for(auto v =match.begin(); v != match.end(); v++)
				{
					sum_match += *v;
				}
				LOG_DEBUG(_log) << "end of branch" <<"match :" << sum_match << "index:" << index <<endl;

				if(t.depth(it_depth)+1 == n && sum_match >= need_to_match)
				{
					LOG_INFO(_log) << "matched "<<endl;
					status = true;
					// print elements in reverse order <stack> => change to deque
					// avoid copy by using other container
					for (std::stack<T> dump = _stack; !dump.empty(); dump.pop())
					{
        				LOG_INFO(_log) << dump.top() << endl;
					}
					//TO DO copy _stack elements to tfound
				}
				end_reached = true;
				last_depth = t.depth(it_depth);
				index = 0;
			}
			it_depth++;
			index++;
	}
	return status;
}



template<class T> class Matrix {
private:
    unsigned _row;
    unsigned _col;
    T * _data;

public:
    Matrix(unsigned row, unsigned col) :_row(row),_col(col){

		unsigned size = _row * _col;
		_data = new T[size]();
	};

	~Matrix()
	{
		delete [] _data;
		cout << "delter" <<endl;
	}

    void printMatrix()
	{
		for(auto i =0; i <_row; i++)
		{
			cout << "[";
			for(auto  j = 0; j < _col; j++)
			{
				cout << _data[i*_col+j] <<" ";
			}
			cout << "]"<< endl;
		}
	}

	unsigned col() const
	{
		 return _col;
	}

	unsigned row() const
	{
		 return _row;
	}

	void set_position(int i, int j, T val)
	{
		_data[i*_col+j] = val;
	}


		// search maximum semi diagonal algo
	std::vector<unsigned> candidates_substitute_erasure()
	{
		std::vector<unsigned> vec;

		for(auto  j = 0; j < _col; j++)
		{
			auto i =0;
			for(; i <_row; i++)
			{
				if( _data[i*_col+j] == 1)
				{
					break;
				}				
			}

			if(i ==_row )
			{
				LOG_INFO(_log) << "candidates_substitute_erasure j: "<<j <<" "<< _col <<  endl;
				vec.push_back(j);
			}
		}

		return vec;
	}


	// search maximum semi diagonal algo
	// to to, search among the same paths the longest diagonal one
	// to maximize substitutions
	std::vector<unsigned> candidates_save()
	{
		std::vector<unsigned> vec;

		for(auto  j = 0; j < _col; j++)
		{
			auto i =0;
			for(; i <_row; i++)
			{
				if( _data[i*_col+j] == 1)
				{
					LOG_INFO(_log) << "candidates_substitute_erasure j: "<<j <<" "<< _col <<  endl;

					vec.push_back(i);
					break;
				}				
			}
		}

		return vec;
	}

	// search maximum semi diagonal algo
	std::vector<std::array<unsigned,2>> by_lines()
	{
		std::vector<std::array<unsigned,2>> vec;
		std::array<unsigned,2> point;

		int min_i = -1;
		int min_j = -1;
	
		// line
		for(auto i = 0; i <_row; i++)
		{
			for(auto  j = 0; j < _col; j++)
			{
				if(i > min_i && j > min_j)
				{
					if( _data[i*_col+j] == 1)
					{
						point[0] = i;
						point[1] = j;
						min_i = i;
						min_j = j;
						vec.push_back(point);

						break;
					}
				}
			}
			
		}

		LOG_INFO(_log) <<"path vec.size() " <<vec.size() << endl;

		// column

		return vec;
	}

	// search maximum semi diagonal algo
	std::vector<std::array<unsigned,2>> by_columns()
	{
		std::vector<std::array<unsigned,2>> vec;
		std::array<unsigned,2> point;

		int min_i = -1;
		int min_j = -1;
	
		// line
		for(auto j =0; j <_col; j++)
		{
			for(auto  i = 0; i < _row; i++)
			{
				if(i > min_i && j > min_j)
				{
					if( _data[i*_row+j] == 1)
					{
						point[0] = i;
						point[1] = j;
						min_i = i;
						min_j = j;
						vec.push_back(point);

						break;
					}
				}
			}
		}

		LOG_INFO(_log) <<"path vec.size() " <<vec.size() << endl;

		return vec;
	}

	// TO DO
	// search maximum semi diagonal algo with zigzag
	std::vector<std::array<unsigned,2>> zig_zag()
	{
		std::vector<std::array<unsigned,2>> vec;
		std::array<unsigned,2> point;
	
		auto preferred_direction = max(_row, _col);

		for(auto i =0; i <_row+_col-1; i++)
		{
			if(_row >= _col )
			{			

			}			
		}

		return vec;
	}


	// search maximum semi diagonal algo
	std::vector<std::array<unsigned,2>> small_squares()
	{
		std::vector<std::array<unsigned,2>> vec;
		std::array<unsigned,2> point;
	
		for(auto i =0; i <_row; i++)
		{
			for(auto  j = 0; j < _col; j++)
			{
				if( _data[i*_col+j] == 1)
				{
					point[0] = i;
					point[1] = j;

					vec.push_back(point);
				}
			}
		}

		return vec;
	}

};


template<class T> bool estimate_cost(std::stack<typename tree<T>::iterator > _stack,int word_length, T word[],int nb_allowed_substitute, int nb_allowed_erasure, int nb_allowed_add)
{
	auto size  =_stack.size();
	T target_word[size] = {};
	auto allowed_search_length = word_length + nb_allowed_add;
	Matrix<int> _matrix(_stack.size(),word_length);
	_matrix.printMatrix();

	// replace with deque, to avoid need to reverse elements 
	std::vector<typename tree<T>::iterator > dump ;
	while(!_stack.empty())
	{
		LOG_DEBUG(_log) <<"stack dump " << *(_stack.top())<<endl;
		dump.push_back(_stack.top());
		_stack.pop();
	}
	std::reverse(dump.begin(), dump.end());

	for(auto i = 0; i <_matrix.row() ; i++)
	{
		T val = *(dump.at(i));
		LOG_INFO(_log) <<"val " <<val << endl;
		for(auto j = 0 ; j < _matrix.col(); j++)
		{
			if(val == word[j])
				_matrix.set_position(i,j,1);
		}
		//dump.pop();
	}
	_matrix.printMatrix();


	// to do best algorithm to match words
	// find maximum "suite" with strict progressive values {1} in sparse matrix
	//the goal is to transfrom this matrix to square one as {identity matrix + some sparse}
	// by respecting the constraint of allowed substitution erasure, add
	// remark : subtition = erasure + add in same position so priority will be given 
	// to substitution
	// example of matrix 
	//		x  t  f  z
	//	e   0  0  0  0
	//	x   1  0  0  0
	//	v   0  0  0  0
	//  n   0  0  0  0
	//  f   0  0  1  0
	//	d   0  0  0  0
	//	f   0  0  1  0

	std::vector<std::array<unsigned,2>> candidates_match1 = _matrix.by_lines();
	LOG_INFO(_log) << " match by lines" << endl;
	for (auto& it :candidates_match1)
	{
		LOG_DEBUG(_log) << (it)[0]  << it[1]<< endl;
	}
	LOG_INFO(_log) << "match by columns" << endl;

	std::vector<std::array<unsigned,2>> candidates_match2 = _matrix.by_columns();
	for (auto & it : candidates_match2)
	{
		LOG_DEBUG(_log) << (it)[0]  << it[1]<< endl;
	}

	std::vector<std::array<unsigned,2>> candidates_match = candidates_match1;
	//  match horizontal vs vertical
	if( candidates_match2.size() > candidates_match1.size())
	{
		candidates_match = candidates_match2;
	}

	auto count_substitute = 0;
	auto count_erasure = 0;
	auto count_add = 0;

	std::array<unsigned,2> init_point {0,0};
	if(candidates_match.size() == 0)
	{
		auto min_size = size < word_length ? size:word_length;
		auto possible_erasure = min_size < nb_allowed_erasure ? min_size: nb_allowed_erasure;
		count_substitute += possible_erasure; 
		if(word_length >= size)
			count_erasure += word_length-size ;	
		else
			count_add += size - word_length;

	}
	else
	{
			
		for(auto  j = 0; j < candidates_match.size(); j++)
		{
			std::array<unsigned,2> point = candidates_match.at(j);
			std::array<unsigned,2> delta_point = {point[0] - init_point[0], point[1] - init_point[1]};
			init_point = point;

			auto min = delta_point[0] < delta_point[1] ? delta_point[0]:delta_point[1];
			auto row_direction = min == delta_point[0];
			auto col_direction = min == delta_point[1];

			auto max = delta_point[0] > delta_point[1] ? delta_point[0]:delta_point[1];

			LOG_DEBUG(_log) <<"j:"<< j << "min:" <<min << "max: " << max << endl;

			// TO DO need to handle budget allowed budget here
			if(max == min)
			{
				if(j==0)
					count_substitute += min;
				else
					count_substitute += min-1;

			}
			else
			{
				if(col_direction)
				{
						if(j==0)
							count_substitute += min;
						else
							count_substitute += min-1;
					count_add += max - min;
				}
				else if(row_direction)
				{
						if(j==0)
							count_substitute += min;
						else
							count_substitute += min-1;
					count_erasure += max - min;			
				}
			}
			// some columns or lines are not processed after last match point
			// in that case add firt possible substitutions
			if (j == candidates_match.size() -1)
			{
				auto last_lines = size -1 -point[0];
				auto last_columns = word_length -1 - point[1];

				auto substitute_end = MIN(last_lines,last_columns);//last_lines < last_columns ? last_lines:last_columns;
		
				auto cnt = 0; // remaining possible substitution
				if(count_substitute < nb_allowed_substitute)
				{
					cnt= nb_allowed_substitute - count_substitute; 
				}
				auto substitute_add_end = MIN(substitute_end,cnt); //substitute_end < cnt ? substitute_end:cnt;

				count_substitute += substitute_add_end;
				count_add += size -1 -point[0] - substitute_add_end; 
				count_erasure += word_length -1 -point[1]  -substitute_add_end; 

			}

		}
	}

	// TO DO 
	// store all matches and return best one by min cost
	// cost == count_substitute + count_add + count_erasure
	LOG_INFO(_log) <<"subsitute:"<<count_substitute <<"add:"<< count_add <<"erasure:"<< count_erasure  <<endl;	
	if ((count_substitute <= nb_allowed_substitute)  &&
		(count_add <= nb_allowed_add) &&
		(count_erasure <= nb_allowed_erasure) )
	{
		LOG_INFO(_log) << "possible match:" << endl;

		for (auto& it :dump)
		{
			LOG_INFO(_log) << *it<< endl;
		}
	}
	else
	{
		LOG_INFO(_log) << "match not possible" << endl;	
	}


	return false;
}

/*
* Version to handle add, erasure of chars  
*/
template<class T>
bool tree_search_add_erase(const tree<T>& ref_tree, int word_length, T word[],int nb_allowed_substitute, int nb_allowed_erasure, int nb_allowed_add, tree<T>& tfound)
{
	auto status = false;
	auto done = false;

	typename tree<T>::pre_order_iterator it_depth,last_it_depth;
	typename tree<T>::iterator parent = nullptr;

	auto current_depth = 0, last_depth = 0;
	auto nb_erased = 0, nb_added = 0, nb_matched =0;
	auto allowed_search_length = word_length + nb_allowed_add + nb_allowed_erasure;
	
	//std::vector<char> browsed_chars;
	std::stack<typename tree<T>::iterator > _visited_nodes;

	// preconditions
	if(nb_allowed_substitute+nb_allowed_erasure>= word_length)
	{
		LOG_WARN(_log) << "Number of allowed substitute:"<< nb_allowed_substitute << " is higher than word_length :"<<word_length <<endl;
		return status;
	}

	it_depth = ref_tree.begin();

	typename tree<T>::iterator next_to_parent =ref_tree.begin();

	while(it_depth != ref_tree.end())
	{

		_visited_nodes.push(it_depth);

		current_depth = ref_tree.depth(it_depth);			
		LOG_DEBUG(_log) << *it_depth <<", depth :"<<ref_tree.depth(it_depth) <<endl;

		// reached end of branch
		if(ref_tree.number_of_children(it_depth) == 0 )
		{ 
			if(ref_tree.depth(it_depth) <= (allowed_search_length-1))
			{
			LOG_DEBUG(_log) << "end of branch and branch depth:" <<ref_tree.depth(it_depth)<<endl;
			LOG_DEBUG(_log) << "Analysis:" <<endl;	
			estimate_cost(_visited_nodes, word_length, word, nb_allowed_substitute, nb_allowed_erasure, nb_allowed_add);
			}
			else
			{
				LOG_WARN(_log) << endl << "Never happen" <<endl;	
			}
			last_it_depth =it_depth;
			it_depth++;
			// empty not needed last elements
			for(auto  _id = ref_tree.depth(it_depth);_id <= ref_tree.depth(last_it_depth) ; _id++)
			{
				_visited_nodes.pop();
			}
		}
		else if(ref_tree.depth(it_depth) >= allowed_search_length-1) // word longer than expected
		{
			LOG_INFO(_log) << "allowed length reached :" <<ref_tree.depth(it_depth)<<":"<<allowed_search_length-1 << endl;

			typename tree<T>::pre_order_iterator current = _visited_nodes.top();
			 _visited_nodes.pop();

			if(_visited_nodes.empty())
			{
				it_depth = ++current;
				LOG_INFO(_log) << "visited nodes empty" << endl;
				continue;
			}
			else
			{
				typename tree<T>::pre_order_iterator parent = _visited_nodes.top();
				_visited_nodes.pop();



				typename tree<T>::sibling_iterator _siblings =current;
				_siblings++;

				
				auto found = false;

				while(ref_tree.end(parent) == _siblings && found == false)
				{
					if(ref_tree.depth(parent) == 0)
					{
						_visited_nodes.pop();
						typename tree<T>::sibling_iterator next_root = parent++;
						it_depth = next_root;		
						break;
					}
					current = parent;
					parent =_visited_nodes.top();
					_visited_nodes.pop();

					typename tree<T>::sibling_iterator _siblings =current;
					_siblings++;
					
					if(_siblings == ref_tree.end(parent))
					{
						LOG_INFO(_log) << "sibling worked" << endl;
					}
					else
					{
						LOG_INFO(_log) << "sibling:" << *_siblings << endl;
					}
					for(typename tree<T>::sibling_iterator it = ref_tree.begin(parent); it!=ref_tree.end(parent); it++)
					{
						LOG_INFO(_log) <<"parent:" << *parent <<  ",it:"  << *it<<" " << endl;
						if(it == _siblings)
						{	found = true;
							break;
						}
					}
				}
				
				it_depth = _siblings;			
				_visited_nodes.push(parent);
				//_visited_nodes.push(it_depth);		
			}	
		}
		else
		{
			it_depth++;
		}
	}
	LOG_INFO(_log) << _visited_nodes.size() <<endl;

}

} // end namespace recursive_search_insert

#endif //_RECURSIVE_SEARCH_
