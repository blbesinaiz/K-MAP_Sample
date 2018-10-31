#include <iostream>
#include <vector>
#include <iomanip>
#include <conio.h>
#include <string>

using namespace std;

//this class is using for filtering results and giving
//all possibilites results
class FilterKmapTerms : public CompareKmapTerms, public ConverteTerms, public Combination 
{
protected:
		
	int	temp, temp1, temp2, temp3; //temprature variables
		
	vector<char> getTerm (vector<char> &result, int pos); //getting a term form an array
	int getMintermCount(vector<char> &term);
	int getLargestTermSize(vector<char> &result);
	vector <vector<char>> getFilterResult(vector<vector < char>> &results, vector<char> &essentialTerms);
	bool checkResult (vector<char> someResult, vector<int> ones );
	vector <vector<char>> filter (vector<char> &result, vector<int> ones);
	
};//end filterKmapTerms

/***
	The main function of the program that implement K-map by:
	1- prompting for k-map type by its variable's number
	2- prompting for one's position by getPos function
	3- prompting for don't care position by getPos function
	4- Minimizing and getting results
*/



//get a term from a vector of terms (result)
vector<char> FilterKmapTerms :: getTerm (vector<char> &result, int pos)
{
	int temp;
	vector<char> term;
	for(temp = pos; temp < result.size(); temp++)
	{
		if(result[temp] != '+')
			term.push_back(result[temp]);
		else
			break;
	}

	return term;
}//end getTerm


	//Determine the larget size of minterms
int FilterKmapTerms :: getLargestTermSize(vector<char> &result)
{
	int largestSize = 0;
	vector<char> term;

	for(int temp = 0; temp  <result.size(); temp += term.size() + 1)
	{
		term = getTerm(result, temp);
		
		if(getMintermCount(term) > largestSize) 
			largestSize = getMintermCount(term);
	
	}//end for

	return largestSize;
}//end get Largest Term size

	//return a term's minterms count
int FilterKmapTerms :: getMintermCount(vector<char> &term)
{
	int count = 0; //for return

	for(int temp = 0; temp < term.size(); temp++)
	{
		if(isalpha(term[temp]) ) //search for alphabets
			count++;
	}
	return count;
}//end get minterms count

vector<vector<char> > FilterKmapTerms :: filter (vector<char> &result, vector<int> ones)
{

	vector<char> term,    //reading all term alone
				essentialTerms; //result after filtering
	vector<vector<char>> filterResult;
	int LargestTermSize;
	vector<vector<char>>terms;
	

	//setting essentials
	LargestTermSize = getLargestTermSize(result);

	for(int temp = 0 ; temp < result.size(); temp ++)
	{
		term = getTerm(result, temp);

		//term is essential 
		if(LargestTermSize > getMintermCount(term) )
		{
			//add + for more than one term result
			if(essentialTerms.size() > 0 )
			{
				essentialTerms.push_back('+');
			}
			for(int temp1 = 0 ; temp1 < term.size(); temp1 ++)
			{
				essentialTerms.push_back(term[temp1]); //add term to filter result
				result.erase(result.begin()+temp);   //remove term form main result
			}

			if(result.size() > 0)//erase + for more than one term
				result.erase(result.begin()+temp);
			
			resultTerms--;  //decrement result terms
			temp--;
		}//end if of essential terms
		else
			temp += term.size();
	}//end result covering loop
	
	terms.push_back(vector<char> () );
	for(int temp = 0; temp < result.size(); temp++)
	{
		if(result[temp] == '+' && temp < result.size())
			terms.push_back(vector<char> () );
		else
		terms[terms.size()-1].push_back(result[temp]);
		
	}

	filterResult = getFilterResult(terms, essentialTerms);

	return filterResult;
}//end filter

/***
*   Getting terms position on k-map by:
*   1- Extracting comparelified terms and return it to 
*      it to its full-minterm terms
*   2- compute its positions by compute position of
*      each full-minterm terms
*/


bool FilterKmapTerms :: checkResult (vector<char> someResult, vector<int> ones)
{
	vector<int> resultPos; //saving someResult positions in k-map

	resultPos = termToPos(someResult,type);

	for(int temp = 0; temp < resultPos.size(); temp++)
	{
		for(int temp1= 0; temp1<ones.size(); temp1++)
		{
		if(ones[temp1] == resultPos[temp])  //searching for matching ones
			ones.erase(ones.begin()+temp1); //erase matching ones
		}
	}

	//results are covering all ones
	if(ones.size() == 0)
		return true;
	else//results don't cover all ones
		return false;

}//end check results

vector<vector<char>> FilterKmapTerms ::getFilterResult(vector<vector < char>> &results, vector<char> &essentialTerms)
{
	vector<vector<char>> filterResult; //return variable
	int filterResultCount = 0;  //count filter results
	int largestCombinationsCount = resultTerms;  //set largest combinations count
	vector<char> someResult;
	vector<vector<int> > possibilites;

	for(int temp = 1; temp <= largestCombinationsCount; temp++)
	{
		possibilites.clear();
		possibilites = getCombination(results.size(), temp);

		
		/* Adding other terms with combinational possibilites */

		//covering possibilites
		for(int temp1 = 0; temp1 < possibilites.size(); temp1++)
		{
			someResult.clear();
			//add essential terms
			for(int temp1 = 0; temp1 < essentialTerms.size(); temp1++)
			someResult.push_back(essentialTerms[temp1]);

			//covering all term alone
			for(int temp2 = 0; temp2 < possibilites[temp1].size(); temp2++)
			{
				
				if(someResult.size() > 0) someResult.push_back('+'); //adding + for separating terms
				for(int temp3 = 0; temp3 < results[ possibilites[temp1][temp2] ].size(); temp3++)
				{
					someResult.push_back(results[ possibilites[temp1][temp2] ] [temp3] );
				}
			
			}//end covering lonly terms

		if(checkResult(someResult,ones) )
		{
			filterResult.push_back( vector<char> () );

			for(int temp1 = 0; temp1 < someResult.size(); temp1++)
			{
				filterResult[filterResult.size() - 1].push_back(someResult[temp1]);
			}

			largestCombinationsCount = temp;
		}//end check result

		}//end covering possibilties

	}//general loop


	
	return filterResult;
}//end get Filter Result

/*
	class ConverteTerms is using to:
	Converte terms to ist equivalent ones

	class Combination is using to:
	provide all combination's possibilites

	class convSopToPos is using to:
	converte terms from some of product type
	to product of some type


*/


class  ConverteTerms 
{
	protected:	

	vector<int> termToPos (vector<char> term, int &type);
	vector <char> extract(vector<char> &terms, int &type);
	void compTerm(vector<char> &part,vector<char> &terms, int &type, int &pos);

};

//complete terms to be a full-minterm terms
void ConverteTerms :: compTerm(vector<char> &part,vector<char> &terms, int &type, int &pos)
	{
		
		int dashCount = 0 ;
		vector<char>copyPart;  //copying part for don't cares
		int temp;
		bool edited = 0; //check edditing in first phase

		pos -= part.size()-1; //return to main positiion
		terms.erase( terms.begin()+pos,terms.begin()+pos+part.size() );//return the part from total terms
		
		//first phase
		for( temp = 0; temp < part.size(); temp++)
		{
			if(part[temp] == '\'')dashCount ++;

			else if (part[temp] != 65 + temp - dashCount)
			{
				part.insert(part.begin()+temp,65+temp-dashCount); //insert letter
				copyPart = part;                                  //copy letter
				part.insert(part.begin()+temp+1,'\'');            //insert ' to the letter
				terms.insert(terms.begin()+pos,copyPart.begin(),copyPart.end()); //insert part without dash
				terms.insert(terms.begin()+pos,'+');                     //insert +
				terms.insert(terms.begin()+pos,part.begin(),part.end()); //insert the part with dash to be first
				edited = true; //this part has eddieted 
				break;

			}
		}
	
		//losing last digit
		if (edited == false &&part[part.size() -1] != 65+type)
		{
				part.insert(part.begin()+temp,65+temp-dashCount); //insert a letter
				copyPart = part;                                  //copy letter
				part.insert(part.begin()+temp+1,'\'');            //insert ' to the letter
				terms.insert(terms.begin()+pos,copyPart.begin(),copyPart.end()); //insert part without dash
				terms.insert(terms.begin()+pos,'+');                     //insert +
				terms.insert(terms.begin()+pos,part.begin(),part.end()); //insert the part with dash to be first
				
		}


	}//end edit part
/***Stretching terms by getting its full-minterm terms by:
*  1- getting comparelified terms
*  2- complete it with "compTerm" Function
*/
vector<char> ConverteTerms :: extract(vector<char> &terms, int &type)
	{
		int temp = 0;
		vector<char>part;  //part of expression
		int dashCount = 0;
		
		//save one term in part
		for(temp = 0; temp < terms.size(); temp++)
		{
			
			if( isalpha(terms[temp]) || terms[temp] == '\'' )
			{
				part.push_back(terms[temp]);
				if(terms[temp] == '\'') dashCount++;
			}	

			 if( (terms[temp] =='+' || temp == terms.size()-1  ) && part.size() - dashCount < type )
			 {
				 if(terms[temp] =='+')
						temp--; //return temp to last digit
				 compTerm(part,terms,type,temp); //complete term
				 temp--;//return temp to the digit before part
				 part.clear(); //clear edit to be used again
				 dashCount=0;
			 }
			 
			 else if (terms[temp] =='+' && part.size() - dashCount == type)
				 {
					 part.clear();  //clear part
					 dashCount=0;   //clear dash
				}

		}//end looping for
		return terms;
	}//end extr


vector<int> ConverteTerms :: termToPos (vector<char> term, int &type)
{
	vector<int>result; //for returning
	int weight = 0; //default case
	int pos = 0; //saving position
	
	term = extract(term, type); 

	//compute a full-minterm term positions
	for(int temp = term.size()-1; temp >=0 ; temp--)
	{
		if(term[temp] == '\'') //dashed digits
		{
			pos +=0;
			temp --; //going to the next minterm
		}
		else if ( isalpha(term[temp] ) ) //undashed digits
		{
			pos += pow(2.0, weight);
		}

		//seperators
		 if(term [temp] == '+' || temp == 0)
		{
			result.push_back(pos);
			pos = 0; //default case
			weight = 0; //default case
			continue;
		}
		weight++;
	}//end for

	for(int temp = 0; temp < result.size() - 1 ; temp++)
	{
		if(result[temp] == result[temp+1])
			result.erase(result.begin()+temp+1);

		else if (result[temp] > result[temp+1])
		{
			swap(result[temp], result[temp+1]);
		temp = -1;
		}
	}

	return result;
}//end term to pos

class Combination
{
	vector<int> people;
	vector<int> element;
	vector <vector<int>> result;
	int  size;
	 int count;

void saveResult(const vector<int>& v, int k, int &size)
{
 
  result.push_back(vector<int> () );
	for (int i = 0; i < v.size(); ++i) 
  {  
	  result[count].push_back(v[i]); 
	  if(result[count].size() == size)
		  count++;
	}

  
}//end save result

void go(int offset, int k) 
{

  if (k == 0) {
    saveResult(element, k, size);
    return;
  }
  for (int i = offset; i <= people.size() - k; ++i) {
    element.push_back(people[i]);
    go(i+1, k-1);
    element.pop_back();
  }
}//end go


public:
	vector<vector<int>> getCombination(int n, int k)
	{
	  size = k;
	people.clear();	
	result.clear();
	count = 0;
	for (int i = 0; i < n; ++i) { people.push_back(i); }
		go(0, k);

 	
  return result;
	}//end setComb

};

//converte from some of product to product ot sum
class SOPtoPOS
{
protected:
	void convSopToPos (vector<char> &result)
	{
		result.insert(result.begin(), '(');
		result.insert(result.end(), ')');

		for(int temp = 0; temp < result.size(); temp++)
		{
			if(result[temp] == '+')
			{
				result.erase(result.begin()+temp);
				result.insert(result.begin()+temp, ')');
				result.insert(result.begin()+temp+1, '.');
				result.insert(result.begin()+temp+2, '(');
				temp ++;
			}
			else if ( temp > 0 (isalpha(result[temp-1]) || result[temp-1] == '\'' ) && isalpha(result[temp] ) 
				&& result[temp-1] != '(' && result[temp-1] != '+' && result[temp] != ')')
			{
				result.insert(result.begin()+temp,'+');
			}
				
		}
	}//end convSopToPos

};

//this class is using for filtering results and giving
//all possibilites results
class FilterKmapTerms : public CompareKmapTerms, public ConverteTerms, public Combination 
{
protected:
		
	int	temp, temp1, temp2, temp3; //temprature variables
		
	vector<char> getTerm (vector<char> &result, int pos); //getting a term form an array
	int getMintermCount(vector<char> &term);
	int getLargestTermSize(vector<char> &result);
	vector <vector<char>> getFilterResult(vector<vector < char>> &results, vector<char> &essentialTerms);
	bool checkResult (vector<char> someResult, vector<int> ones );
	vector <vector<char>> filter (vector<char> &result, vector<int> ones);
	
};//end filterKmapTerms

/***
	The main function of the program that implement K-map by:
	1- prompting for k-map type by its variable's number
	2- prompting for one's position by getPos function
	3- prompting for don't care position by getPos function
	4- Minimizing and getting results
*/



//get a term from a vector of terms (result)
vector<char> FilterKmapTerms :: getTerm (vector<char> &result, int pos)
{
	int temp;
	vector<char> term;
	for(temp = pos; temp < result.size(); temp++)
	{
		if(result[temp] != '+')
			term.push_back(result[temp]);
		else
			break;
	}

	return term;
}//end getTerm


	//Determine the larget size of minterms
int FilterKmapTerms :: getLargestTermSize(vector<char> &result)
{
	int largestSize = 0;
	vector<char> term;

	for(int temp = 0; temp  <result.size(); temp += term.size() + 1)
	{
		term = getTerm(result, temp);
		
		if(getMintermCount(term) > largestSize) 
			largestSize = getMintermCount(term);
	
	}//end for

	return largestSize;
}//end get Largest Term size

	//return a term's minterms count
int FilterKmapTerms :: getMintermCount(vector<char> &term)
{
	int count = 0; //for return

	for(int temp = 0; temp < term.size(); temp++)
	{
		if(isalpha(term[temp]) ) //search for alphabets
			count++;
	}
	return count;
}//end get minterms count

vector<vector<char> > FilterKmapTerms :: filter (vector<char> &result, vector<int> ones)
{

	vector<char> term,    //reading all term alone
				essentialTerms; //result after filtering
	vector<vector<char>> filterResult;
	int LargestTermSize;
	vector<vector<char>>terms;
	

	//setting essentials
	LargestTermSize = getLargestTermSize(result);

	for(int temp = 0 ; temp < result.size(); temp ++)
	{
		term = getTerm(result, temp);

		//term is essential 
		if(LargestTermSize > getMintermCount(term) )
		{
			//add + for more than one term result
			if(essentialTerms.size() > 0 )
			{
				essentialTerms.push_back('+');
			}
			for(int temp1 = 0 ; temp1 < term.size(); temp1 ++)
			{
				essentialTerms.push_back(term[temp1]); //add term to filter result
				result.erase(result.begin()+temp);   //remove term form main result
			}

			if(result.size() > 0)//erase + for more than one term
				result.erase(result.begin()+temp);
			
			resultTerms--;  //decrement result terms
			temp--;
		}//end if of essential terms
		else
			temp += term.size();
	}//end result covering loop
	
	terms.push_back(vector<char> () );
	for(int temp = 0; temp < result.size(); temp++)
	{
		if(result[temp] == '+' && temp < result.size())
			terms.push_back(vector<char> () );
		else
		terms[terms.size()-1].push_back(result[temp]);
		
	}

	filterResult = getFilterResult(terms, essentialTerms);

	return filterResult;
}//end filter

/***
*   Getting terms position on k-map by:
*   1- Extracting comparelified terms and return it to 
*      it to its full-minterm terms
*   2- compute its positions by compute position of
*      each full-minterm terms
*/


bool FilterKmapTerms :: checkResult (vector<char> someResult, vector<int> ones)
{
	vector<int> resultPos; //saving someResult positions in k-map

	resultPos = termToPos(someResult,type);

	for(int temp = 0; temp < resultPos.size(); temp++)
	{
		for(int temp1= 0; temp1<ones.size(); temp1++)
		{
		if(ones[temp1] == resultPos[temp])  //searching for matching ones
			ones.erase(ones.begin()+temp1); //erase matching ones
		}
	}

	//results are covering all ones
	if(ones.size() == 0)
		return true;
	else//results don't cover all ones
		return false;

}//end check results

vector<vector<char>> FilterKmapTerms ::getFilterResult(vector<vector < char>> &results, vector<char> &essentialTerms)
{
	vector<vector<char>> filterResult; //return variable
	int filterResultCount = 0;  //count filter results
	int largestCombinationsCount = resultTerms;  //set largest combinations count
	vector<char> someResult;
	vector<vector<int> > possibilites;

	for(int temp = 1; temp <= largestCombinationsCount; temp++)
	{
		possibilites.clear();
		possibilites = getCombination(results.size(), temp);

		
		/* Adding other terms with combinational possibilites */

		//covering possibilites
		for(int temp1 = 0; temp1 < possibilites.size(); temp1++)
		{
			someResult.clear();
			//add essential terms
			for(int temp1 = 0; temp1 < essentialTerms.size(); temp1++)
			someResult.push_back(essentialTerms[temp1]);

			//covering all term alone
			for(int temp2 = 0; temp2 < possibilites[temp1].size(); temp2++)
			{
				
				if(someResult.size() > 0) someResult.push_back('+'); //adding + for separating terms
				for(int temp3 = 0; temp3 < results[ possibilites[temp1][temp2] ].size(); temp3++)
				{
					someResult.push_back(results[ possibilites[temp1][temp2] ] [temp3] );
				}
			
			}//end covering lonly terms

		if(checkResult(someResult,ones) )
		{
			filterResult.push_back( vector<char> () );

			for(int temp1 = 0; temp1 < someResult.size(); temp1++)
			{
				filterResult[filterResult.size() - 1].push_back(someResult[temp1]);
			}

			largestCombinationsCount = temp;
		}//end check result

		}//end covering possibilties

	}//general loop


	
	return filterResult;
}//end get Filter Result

/***Karnaugh map
* This program make a simulation to Karnaugh map with any number of variables
* and minimizing it
*/

/***
* Program algorithm
* 1- Reading inputs (ones and don't care) from user 
* 2- Starting comparing ones and don't care 
* 3- Converte comparelified result to alphabetical terms 
* 4- filter results to avoid excess terms and provde all possible slovings
*/

//this class is using for implementing k-map by
//following algorithm
class Kmap : public FilterKmapTerms, public SOPtoPOS
{
private:
		int	temp, temp1, temp2, temp3; //temprature variables
		
	void invokeKmap();
	
public:
	
	Kmap () 
	{
	    invokeKmap();
	}
};//end kmap

/***
	The main function of the program that implement K-map by:
	1- prompting for k-map type by its variable's number
	2- prompting for one's position by getPos function
	3- prompting for don't care position by getPos function
	4- Minimizing and getting results
*/

void Kmap ::invokeKmap()
	{
		char tempChar;           //temprature character 
		vector<char>result;      //Saving results
		vector<vector<char>> filterResults; //results after being filtered
	do
	{
		
		//first step prompting for type

		system("cls");  //clear windows
		guideWin(1);     //invoke guide windows
		
		//prompting for type
		cout<<"Please, enter your k-map type (Variables number, Maximum 26):";
		do
		{
		type = readInt( temp = 0, false); //read type as an positive integer number
		
		if(type > 26)
			for( int temp1=0; temp1 < temp; temp1++)
				cout<<"\b \b";

		}while(type > 27);
		hasEnteredType = true;

		//second step prompting for ones
		system("cls");  //clear windows
		guideWin(2);     //reinvoke guide windows
		getPos(ones, "one");           //getOnes

		//third step prompting for don't care
		system("cls");  //clear windows
		guideWin(3);     //reinvoke guide windows
		getPos(dCare, "don't care");   //get don't care

		//fourth step prompting for don't care
		system("cls");  //clear windows
		guideWin(4);     //reinvoke guide windows
		cout<<"Choose Result's type:\n"
			<<"1-Some of product.\n"
			<<"2-Product of some.\n";
		
		do
		{
		tempChar = getch();
		
		if(tempChar == 49)
		{
			SOP = true;
		}
		else if(tempChar == 50)
		{
			SOP = false;
		}
		else
		{
			cout<<"\a";
		}

		}while(tempChar != 49 && tempChar != 50);
		//fifth step: getting results
		system("cls");  //clear windows
		guideWin(5);     //reinvoke guide windows
		result = minimize(ones, dCare); //Solving by minimizing
		
		if( result.size() != 1 ||   //not full or empty maps
			( result[0] != 48 && result[0] != 49 ) )
		{
			filterResults.clear();
			filterResults =  filter(result,ones); //filter result from unessential terms
			//print results
		cout<<"Minimization = ";
		
		for(int temp = 0; temp < filterResults.size(); temp++)
		{
			if(filterResults.size() > 1) 
				cout<<endl<<temp+1<<" - ";

			if(SOP == false) convSopToPos(filterResults[temp]);

			for(int temp1 = 0; temp1 < filterResults[temp].size(); temp1++) 
			cout<<filterResults[temp][temp1];
		}

		}
		else
			cout<<"Minimization = "<<result[0]<<endl;
		
				cout<<endl; //going to next line
		
		tempChar = getch ();       //reading a digit for finishing minimize
		if(tempChar == 27) break;  //break after esc
		else
		{
			//empty type, ones and dCare
			ones.clear();
			dCare.clear();
			hasEnteredType = false;
		}
	}while(true);
	
}//end invoke kmap



void main ()
{
	
	Kmap k1;
	
}//end main
