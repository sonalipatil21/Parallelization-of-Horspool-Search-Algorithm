# HorspoolSearch

## Proposal
This project looks to parallelize the Boyer Moore Horspool algorithm for text search (stringmatching) over large volumes of text.

## Boyer Moore Horspool algorithm
The Boyer Moore Horspool algorithm is an algorithm for finding substrings within strings through comparing each character within the substring to find matching characters and return the match. This algorithm is known to be the fastest way to search for substrings and is usually utilized in search bars, auto-corrects, and Big Data searches. 

The algorithm works to reduce the number of CPU cycles and search time necessary to find a given text. The execution time is linear as it looks at the size of the string being searched which makes it have a lower execution time factor in comparison to that of other search algorithms. The worst-case performance of the Horspool algorithm is also less than other search algorithms as it looks to have a runtime of O(mn) where m is the length of the text being searched for and n is the text being searched. 

## How It Works
The Horspool algorithm works to compare an order of characters within a substring with those within a string to find a potential match. When the order of characters from the substring don't match those in the string, the substring jumps to search from the next position in the pattern found through the value indicated within a Bad Match Table. 

The Bad Match table is created through calculating the value of each character within the substring with the formula 


                                Value = length of substring – index of each letter in the substring – 1 


The last character and charactersnot within the substring are given the value of the length of the substring. To compare the order of characters in a substring with those in a string, you start from the index of the last character in the substring. If the character matches with its string position character, then the preceding characters are compared as well. If it does not match, or any preceding characters do not match, the value corresponding to the last character is found through the Bad Match Table and the substring skips that many given number of spaces indicated. This is done until a total match is found and the substring is located within the string.
