''' Zipf Calculator FASTER?
    Zack Crenshaw
    7/7/16

    Calculating frequency of words in a given text

    Inspired by Zipf's Law (more info in link below)

    https://www.youtube.com/watch?v=fCn8zs912OE
'''

# version 7: file writing is broken right now

import time

def main():
    curve = []
    restart = "y"
    the_counter = 0
    the_avg = 0

    while restart == "y":
        c = [] #count of occurences list
        w = [] #word list
        errors = []
        #input
        fileName = input("Input a filename, with extension (should be .txt) ")
        f = open(fileName, "r")
        s = f.read()
        a = s.split()

        # simplify words
        for i in range (len(a)):
            a[i] = a[i].strip(".")
            a[i] = a[i].strip("!")
            a[i] = a[i].strip("?")
            a[i] = a[i].strip(",")
            a[i] = a[i].strip("(")
            a[i] = a[i].strip(")")
            a[i] = a[i].strip('"')
            a[i] = a[i].strip(" ");
            a[i] = a[i].lower()
        
            total = len(a)
        #get number of occurences
        for i in a:
            if i not in w:
                x = a.count(i)
                c.append(x)
                c.sort()
                c.reverse()
                index = c.index(x)
                w.insert(index,i)
                for j in range(x):
                    #print (i)
                    a.remove(i)
        f.close()

        #data display
    ##    print "\nProjected data:"
    ##    for n in range(100):
    ##        rank = 1.0/(n+1)
    ##        projected = round(rank*c[0],0)
    ##        print projected
    ##    
    ##    print "\nActual Data:"               
    ##    for n in range(100):
    ##        print c[n]

     #curve projection
        initial = c[0]
        totError= 0
        error = 0
        for n in range(1,100):
            for place in range(1,5):
                pro = initial*(1/(place+1)**(n/100.0))
                error = (pro-c[place])*100.0/c[place]
                totError = error +totError
            errors.append(abs(totError/100))
            totError = 0
        error = 100 - errors.index(min(errors))

##        print initial
##        for p in range(2,51):
##            print round(initial*(1.0/(p**ex)),0)





        
        tottime = time.process_time()
        #output
        print ("\n10 Most common words in text: \n")
        if len(w)>9:
            for h in range(10):
    ##            if h>0:
    ##              r=1.0/(h+1) #1/rank
    ##              z=r*c[0] #proper value for rank
    ##              print w[h], " ", c[h], "  Percent difference from Zipf's Law: ", round((c[h]-z)*100/z,2), "%"
    ##            else:
                   print (w[h], " ", c[h])
        else:
            for j in range(len(w)):
                print (w[j], " ", c[j])

        print ("\nTotal Words: ", total)
        print ("Total Unique words: ", len(w)) 
        print ("Most common word: ", w[0] ,"(",c[0],"ocurrences )")
        print ("Least common word: ", w[-1], "(",c[-1],"ocurrence )")
        print ("Most common word '", w[0], "' as percentage of language in text: ", round(c[0]*100.0/total,2),"%")

        #processing time
        
        print ("\nTime to process: ", round(tottime,2) , " seconds")
        print ("Processing rate: ", round(total/tottime ,2) , " words per second\n")
        #curve data output
        ex = 1-(error/100.0)
        print ("\nCurve exponent for this data set",ex,"\n")
        
        # "the" metrics       
        if w[0] == "the":
            the_avg = round(c[0]*100.0/total,2) + the_avg
            the_counter = the_counter + 1
        # average curve data
        curve.append(ex)
        avg_curve = sum(curve)/len(curve)

        ##for i in w:
          ##  print (i)
        ##for i in a:
          ##  print (". "+i)



        #file writing:

##        data_counter = 0
##        data_sum = 0
##        #data saving
##        with open("translingual_data.txt", "r+") as f:
##            data = f.readlines()
##            for i in range(2,len(data)):
##                if i%2 == 0:
##                    data_sum = data_sum + float(data[i])
##                    data_counter = data_counter + 1
##            data_sum = data_sum +ex
##            data_counter = data_counter + 1
##            data_avg = data_sum/data_counter
##            print "\nNew average translingual exponent: ",data_avg
##
##            #saving
##            addition = fileName + "\n" + str(ex)
##            data.append(addition)
## ##           data.remove("TRANSLINGUAL DATA")
##            data.remove("Current Trans-lingual average:")
##            for each in data:
##                f.write(each)
##                data.remove(each)
            
            
        #restart
        restart = input("Again? ")
        restart = restart.lower()


main()



