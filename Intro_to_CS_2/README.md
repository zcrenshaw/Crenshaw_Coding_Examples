# Intro_to_CS_2

PRIMARY LANUGAGE: C

This course focused on data structures, written in C. These include linked lists, doubly linked lists, arrays, array lists, binary search trees, hashtables, and graphs. We also learned how to use debuggeres such as Valgrind. A full description is available at http://collegecatalog.uchicago.edu/thecollege/computerscience/ 
under CMSC 15200: Introduction to Computer Science-2.


CS152coin: Mid-term project. Simulates a blockchain. The program cs152coin parses the blockchain data in blockchain-short.txt, runs basic statistics, tracks address balances, and detects double spending. To run:

./cs152coin ADDRESS < blockchain-short.txt

ADDRESS is some address from blockchain-short.txt. Example:

./cs152coin 321a34b5fb3e8ca5f53e87b6a95aed1 < blockchain-short.txt


CS152coin_part2: Final project. Builds on the mid-term project. Applies a deanonymization algorithm to a longer blockchain. To run:

./cs152coin_part2 ./cs152coin_part2 <blockchain-deanon.txt ADDRESS

For ID1 and ID2, use some addresses from the blockchain. Example:

./cs152coin_part2 <blockchain-deanon.txt f716bd6a36845f3275739e398e397828 

Other folders include homeworks with implimentations of the data structures in the title.