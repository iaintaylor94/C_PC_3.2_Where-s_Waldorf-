# Programming Challenges
## Problem 3.2: Where's Waldorf[^1]
### Problem description
This program finds words within a "word grid" - essentially a two dimensional array of characters. The words to find, and their locations in the grid, are stored in a dictionary. The case of the letters does not matter.

The input states the number of cases to process on a line followed by a blank line. Then the dimensions of the word grid (1 <= m,n <= 50), and the contents of the word grid - line by line. Also, the number of words (1 <= k <= 20) and the contents of the dictionary, again one per line.


### Example 
>**Input**                        
>1                              
>                      
>8 11                         
>abcDEFGhigg                    
>hEbkWalDork                           
>FtyAwaldORm                     
>FtsimrLqsrc                      
>byoArBeDeyv                       
>Klcbqwikomk                       
>strEBGadhrb                           
>yUiqlxcnBjf                         
>4                        
>Waldorf                               
>Bambi                           
>Betty                              
>Dagbert                                                                                                                                                                    

>**Output**                                             
>2 5                                                      
>2 3                          
>1 2                               
>7 8                                                                                                                                     

### Problem solution
A grid containing the letters input from the file will be created, padded with '\0' on the border. The program will search the rows, collumns and diagonals of all lines beginning on the top or left edge of the grid. If a word is found, its location (stored in the dictionary) will be set to the appropriate value. Finally the locations of the words will be printed to the terminal.

### Bug Checklist

[^1]: Programming Challenges By S.Skienna and M.Revilla, page 67