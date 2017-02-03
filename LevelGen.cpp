////////////////////////////////////////////////////////////////
//sudoku.dll(c) 2005 Amiya Patanaik.
//DLL written in visual c++ .net 2005.
//
//creator: Amiya Patanaik. 
//  Ideas: Taken from sudoku python script by Arel Cordero
//   date: Nov 30, 2005
//
// description: On the fly generation of sudoku.
//				The sudoku is generated as a string.
//	   Example: If the generated sudoku is...
//				| |1|4|7|5| |9|8| |     First ROW
//				| |3| | | |.......			.
//			    ..................			.
//				............| |7| |			.
//				| |9|8| |6|4|2|3| |     Last ROW
//
//				It will be encoded as...
//'0,1,4,7,5,0,9,8,0,0,3,0,0,0,........,0,7,0,0,9,8,0,6,4,2,3,0'
//              
//   arguments: argument0:String that will hold the generated sudoku.
//						  This string is passed by reference. So it 
//						  must be large enough to hold the sudoku-that is
//						  atleast 85 characters long.
//				argument1:Max tries-The maximum number of tries the computer
//						  will execute to generate the desired sudoku.
//						  If the desired one is very difficult one then 
//						  specifying a large 'max try' may cause the computer
//						  to hang!!
//				argument2:With cells-No of visible cells in the genarated sudoku.
//						  A solved sudoku has 81 cells. An average level sudoku
//						  has 35 cells. Generating a sudoku with less than 25 cells
//						  is very difficult and may not be possible even
//						  when Max tries = 10000 !!
//							it took 8 minutes to process on my system :<>
//     Returns: 1 if process is succesful.
//              
//     remarks: In case the dll is unable to generate the desired sudoku even after
//				no. of tries = Max tries it simply returns the best sudoku generated.
//				Take care that the dll file and gm6/gb
//              file are in the same folder.Beware passing
//              invalid arguments may cause problems or in rare
//              cases a general protection fault!!
////////////////////////////////////////////////////////////////

// LevelGen.cpp : Defines the entry point for the DLL application.
#pragma warning (disable:4786)
#include "stdafx.h"
#include<stdio.h> 
#include<stdlib.h> 
#include<time.h> 
#include<conio.h>
#include<vector> //preferred container for a sequence
				//when random-access performance is at a premium.
using namespace std ;
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

typedef vector<int> sample; //Type define type vector
int TheBestMatrix[9][9]; //Holds The best possible generated sudoku.

bool Is_Valid(int matrix[9][9],int row,int col,int my_row,int my_col);
int pluck(int matrix[9][9],int level);
int get_me_the_best(int matrix[9][9],int runs,int with_cells);
double generate_puzzle(char* out_matrix);
int check_this_in_a_col(int matrix[9][9],int col,int maxindex,int element); 
int is_this_in_3by3cell(int value,int filled[9]); 
void initialise_filled_to_0(int filled[9]); 
int make_a_new_cell(int matrix[9][9],int rl,int rh,int cl,int ch,int how2check); 
int find_dup(int matrix[9][9],int i,int j,int array[9]); 
int check_cell(int matrix[9][9],int rl,int rh,int cl,int ch); 
void actual_thing_from_matrix(int matrix[9][9],int level); 

int pluck(int matrix[9][9],int level)
{
	sample cells;
	sample cellsleft;
	//sample::size_type _size;
	int cell,my_row,my_col,row,col,square,my_sq_row,my_sq_col,cells_size = 81;
	for(int i=0;i<81;++i)
	{
		cells.push_back(i);
		cellsleft.push_back(i);
	}
	//_size = cellsleft.size();
	while(!cellsleft.empty()&& cells_size > level)
	{
		int &iter = cellsleft.at(rand()%(int)cellsleft.size());
		cell = iter;
		cellsleft.erase(&iter);
		//cout<<cell<<endl;
		//cin>>my_x;
		row=col=square=0;
		my_row=int(cell/9);
		my_col=int(cell%9);
		my_sq_row = int(my_row/3)*3;
		my_sq_col = int(my_col/3)*3;

		for(i=0;i<9;++i)
		{
			if(i!=my_row)
				if(Is_Valid(matrix,i,my_col,my_row,my_col)) col = 1;
			if(i!=my_col)
				if(Is_Valid(matrix,my_row,i,my_row,my_col)) row = 1;
		}
		for(int m=0;m<3;++m)
		{
			for(int n=0;n<3;++n)
			if(!(m+my_sq_row==my_row && n+my_sq_col==my_col))
				if(Is_Valid(matrix,m+my_sq_row,n+my_sq_col,my_row,my_col)) square = 1;
		}
		if(row && col && square) continue;
		matrix[my_row][my_col] = 0;
		cells_size -= 1;
	}
		
	return cells_size;

}

//Answers the question: can the cell (i,j) in the puzzle "puz" contain the number
//in cell "c"? """
bool Is_Valid(int matrix[9][9],int row,int col,int my_row,int my_col)
{
	if(matrix[row][col] != 0) return false;
	int my_sq_row = int(row/3)*3;
	int my_sq_col = int(col/3)*3;
	int value_of_matrix=matrix[my_row][my_col];
	if(matrix[row][col]==value_of_matrix)return true;
	for(int i=0;i<9;++i)
		{
			if(i!=my_row)
				if(matrix[i][col]==value_of_matrix) return false;
			if(i!=my_col)
				if(matrix[row][i]==value_of_matrix) return false;
		}
		for(int m=0;m<3;++m)
		{
			for(int n=0;n<3;++n)
			if(!(m+my_sq_row==my_row && n+my_sq_col==my_col))
				if(matrix[m+my_sq_row][n+my_sq_col]==value_of_matrix) return false;
		}
	return true;
}

int get_me_the_best(int matrix[9][9],int runs,int with_cells)
{
	int best=81;//81 is the worst!!
	int How_Good,i,j,k;
	int HoldMatrix[9][9];
	for(k=1;k<runs;k++)
	{
        for(i=0;i<9;++i)
		for(j=0;j<9;++j)		//Initialise the hold matrix
			HoldMatrix[i][j] = matrix[i][j];
		How_Good = pluck(HoldMatrix,with_cells);
		if(best>How_Good)
		{
		for(i=0;i<9;++i)
		for(j=0;j<9;++j)
			TheBestMatrix[i][j]=HoldMatrix[i][j];
		best = How_Good;
		if(best == with_cells) return best;
		}
	}
	return best;
}

double generate_puzzle(char* out_matrix,double max_tries,double with_cells) 
//Double Bcoz GML doesn't support int data types
{
	int i,j,k,value,filled[9],matrix[9][9];
	srand( (unsigned)time( NULL ) );  //seeds the random number generator 
	initialise_filled_to_0(filled); 
/*
Creates A valid SuDoku i.e. A 9*9 matrix with every row and column have 
elements from 1-9 and no row and col has any num repeated also every 
3*3 cell consists of elements from 1-9 and not repeated 
also do-while loops make the cells to be recreated if a conflict is found 
(in row col or in 3*3 cell of the sudoku) i.e backtracking if a row col or 3*3cell 
 conflict is found 
*/ 

for (i=0;i<3;i++) 
        { 
        for(j=0;j<3;j++) 
        { 
        do{ 
         value=rand()%9+1;//random numbers between 0->(9-1) are produced so we add 1 since we want 1to9 
         }while(is_this_in_3by3cell(value,filled)==0); 

        matrix[i][j]=value; 
        } 
} 

do{ 

   make_a_new_cell(matrix,0,3,3,6,1); 

   }while(make_a_new_cell(matrix,0,3,6,9,1)==0); 

do{ 
   make_a_new_cell(matrix,3,6,0,3,2); 
   make_a_new_cell(matrix,3,6,3,6,3); 

      }while(make_a_new_cell(matrix,3,6,6,9,3)==0); 

do 
{ 
   do 
   { 
      make_a_new_cell(matrix,6,9,0,3,2); 

         }while(make_a_new_cell(matrix,6,9,3,6,3)==0); 
         }while(make_a_new_cell(matrix,6,9,6,9,3)==0); 

//Matrix generated
srand( (unsigned)time( NULL ) );  //randomizes otherwise random and rand always give same op on each run 

//Randomly pluck out cells (numbers) from the solved puzzle grid,
//ensuring that anyplucked number can still be deduced from the remaining cells.
//For deduction to be possible, each other cell in the plucked number's row, column,
//or square must not be able to contain that number
//function get_me_the_best does the work.

get_me_the_best(matrix,(int)max_tries,(int)with_cells);
k = 0;
char buffer[2];
for(i=0;i<9;i++) 
    {
		for(j=0;j<9;j++) 
		{
			_itoa(TheBestMatrix[i][j],buffer,10);
			out_matrix[k]=buffer[0];
			out_matrix[k+1]=',';
			k++;k++;
		} 
	}  
return 1;//At last success
}


/*this function checks if the 3*3 cell contains 
unique numbers between 1-9 for first (initial)3*3 cell*/ 
int is_this_in_3by3cell(int value,int filled[9]) 
{ 
int i; 
   for(i=0;i<9;i++) 
      { 
      if(filled[i]==value) 
      return 0; 

      } 

   for(i=0;i<9;i++) 
    { 
   if(filled[i]==0) 
     { 
      filled[i]=value; 
      break ; 

     } 
   } 
return 1; 
}//end func 

//initialises filled[] array to zero 
void initialise_filled_to_0(int filled[9]) 
{ 
int i; 
for (i=0;i<9;i++) 
filled[i]=0; 
} 

//This function Checks the occurance of a number in a column specified 
//and returns 0 if the element is present 

int check_this_in_a_col(int matrix[9][9],int col,int maxindex,int element) 
{ 

int i; 

   if(element==0) 
   return 0; 

   for(i=0;i<maxindex;i++) 
   { 
      if (matrix[i][col]==element) 
      return 0; 

      } 
      return 1; 
} 


//This function Checks the occurance of a number in a row specified 
//and returns 0 if the element is present 

int check_this_in_a_row(int matrix[9][9],int row,int maxindex,int element) 
{ 
int i; 

   if(element==0) 
   return 0; 

   for(i=0;i<maxindex;i++) 
      { 
      if (matrix[row][i]==element) 
      return 0; 

         } 
      return 1; 

} 

/*This function creates a new 3*3 cell with unique numbers from 1->9 
and also calls functions check_this_in_a_row() and check_this_in_a_col() 
so as to find if the element exists in the row or column of  SuDoku 
yet generated  the option how2check specifies while creating a cell (3*3) 
whether only horizontal check is to be done ( reqd for cell2 and cell3) 
or only vertical(reqd for cell 4 and cell 7) or both checks to be done 
(cells 5,6,8,9) 
*/ 

int make_a_new_cell(int matrix[9][9],int rl,int rh,int cl,int ch,int how2check) 

{ 

int i,j,presentcol=0; 
int available_elements[9]={1,2,3,4,5,6,7,8,9}; 
int value; 
int max_lim=0,breakflag=0; 

   switch(how2check) 
   { 
   case 1: 

      do{ 

      for(i=rl;i<rh;i++) 
      { 
         for(j=cl;j<ch;j++) 
         { 
            do{ 
            value=rand()%9; 
                  max_lim++; 
            if(max_lim>30) 
               { 
                  breakflag=1; 
               break; 
                } 
               }while(check_this_in_a_row(matrix,i,j,available_elements[value])==0); 
   if(breakflag==1) 
   { 
   return 0; 
   } 
   matrix[i][j]=available_elements[value]; 
   max_lim=0; 
         }//for j 

         }//for i 
      }while (check_cell(matrix,rl,rh,cl,ch)==0); 

         break; 

case 2: 

   do{ 
      for(i=rl;i<rh;i++) 
      { 
      for(j=cl;j<ch;j++) 
         { 
         do{ 
         value=rand()%9; 
         max_lim++; 
         if(max_lim>30) 
            { 
            breakflag=1; 
            break; 
            } 
            }while(check_this_in_a_col(matrix,presentcol+j,i,available_elements[value])==0); 
               if(breakflag==1) 
            { 
            return 0; 
               } 
         matrix[i][j]=available_elements[value]; 
         max_lim=0; 
   }//for j 

   }//for i 
   }while (check_cell(matrix,rl,rh,cl,ch)==0); 

      break; 

case 3: 

   do{ 
      for(i=rl;i<rh;i++) 
      { 
      for(j=cl;j<ch;j++) 
         { 
      do{ 
         value=rand()%9; 
      max_lim++; 
      if(max_lim>30) 
         { 
         breakflag=1; 
         break; 
         } 
         }while(check_this_in_a_col(matrix,presentcol+j,i,available_elements[value])==0||check_this_in_a_row(matrix,i,j,available_elements[value])==0); 
         if(breakflag==1) 
         { 
         return 0; 
      } 
         matrix[i][j]=available_elements[value]; 
      max_lim=0; 
      }//for j 

      }//for i 
      }while (check_cell(matrix,rl,rh,cl,ch)==0); 

         break; 

      } 
   return 1; 
         } 


//This Function Checks For Whether NO of elements in a 3*3 cell are from 1-9 
//and not repeated 

int check_cell(int matrix[9][9],int rl,int rh,int cl,int ch) 
{ 
int i,j; 
int this_array[9]={1,2,3,4,5,6,7,8,9}; 

for(i=rl;i<rh;i++) 

   { 
      for(j=cl;j<ch;j++) 
      { 
         if(matrix[i][j]==0) 
         return 0; 
         if(find_dup(matrix,i,j,this_array)==0) 
            return 0; 
      } 
   } 
return 1; 
} 

//finds duplicate element within a cell 

int find_dup(int matrix[9][9],int i,int j,int array[9]) 
{ 

if(array[matrix[i][j]-1]!=0) 
      array[matrix[i][j]-1]=0; 
else 
   { 
   return 0; 
   } 
return 1; 
} 
