#Language Game
#Zack Crenshaw
#6/12/15

import time
import random

def instructions(): #intro
    print ("Welcome to Guess the Language.")
    print ("In this game, you match the sample text to the language.")
    print ("You'll have 10 texts to match.")
    print ("Your score will depend on the number of correct guesses and on the time to answer the questions.")
    print ("Try to be in the top ten!")

def setup(L,T,used,score):  #game function
    text=0
    lang=0
    check=1
    z=[]
    rand=[]
    while check==1:     #check to see if quote has been used
        n=random.randint(0,len(L)-1)
        text=T[n]
        lang=L[n]
        if n not in used:
            used.append(n)
            check=0
    print ("\n",text,"\n")
    check=1
    while len(rand)<4:  #creates the random author list
        check=1
        while check==1:
            m=random.randint(0,len(L)-1)            #m is random index
            if m not in z:  #checking if used           #z is list of used languages for this round
                if L[m] !=L[n]:
                    z.append(m)
                    if L[m] not in rand:                        #rand is list of random languages
                        rand.append(L[m]) 
                        check=0
    rand.append(lang)
    rand.sort()
    for i in range(len(rand)):
        print (i+1," ",rand[i])
    print
    check=1
    while check==1: #bulletproofing
        ans=input("What language is this? (Choose 1-5): ")
        ans=eval(ans)
        ans=ans-1
        if ans>-1 and ans<len(rand):
            check=0
        else:
            print("Please input a vaild number (1-5)")
            
    print ("You chose:", rand[ans])
    if rand[ans]==lang:               #check the answer
        print ("Yes! Nice job.")
        score=score+1
    else:
        print ("Darn. The real answer was:", lang)
    return score
    return used

def finale(score,name,tottime): #ending program
    nums=[]
    names=[]
    hssort=[]
    f=open("high_scores.txt","r")
    print ("The score is determined by the number of correct answers (out of 10) multiplied by 100, minus the time it took for you to answer.")
    print ("The higher the score, the better!")
    hs=f.readlines()
    f.close()
    newscore=score*100
    newscore=int(newscore-tottime) #score calculation
    for i in hs: #list maker
        L=len(i)-1          
        p=i.index("%")
        x=i[:p]
        y=i[p+1:L]
        x=eval(x)
        nums.append(x)
        names.append(y)
    nums.append(newscore)
    nums.sort(reverse=True)
    place=nums.index(newscore)
    names.insert(place,name)
    nums.sort(reverse=True)
    print ("\nYour score:", newscore)
    print ("\nTop Ten scores:\n")
    check=0 
    for i in range(10): #top ten printer
        k=i+1
        print (names[i],"    Score:",nums[i])
        if nums[i]>=250:
            if nums[k]<=250:
                if check<3:
                    print ("BRONZE MEDAL   Score: 250") #to show score zones
                    check=3
        if nums[i]>=500:
            if nums[k]<=500:
                if check<2:
                    print ("SILVER MEDAL   Score: 500")
                    check=2
        if nums[i]>=750:
            if nums[k]<=750:
                if check<1:
                    print ("GOLD MEDAL    Score: 750")
                    check=1
    f=open("High_scores.txt","w")
    for i in range(10): #top ten writer
        string=str(nums[i])
        load=string+"%"+names[i]
        f.write(load+"\n")
    f.close()
    
def main():
    T=[]
    L=[]
    used=[]
    score=0
    instructions() #beginning
    name=input("What is your name? ")
    f=open("Lang.txt","r")
    whole=f.readlines()
    f.close()
    for i in range (len(whole)): #setting up lists
        a=whole[i]
        e=len(a)-1
        p=a.index("%")
        text=a[:p]
        lang=a[p+1:e]
        lang.strip()
        L.append(lang)
        T.append(text)
    L[-1]=L[-1]+'h' #glitch fix
    check=1
    while check==1:
        a=input("Are you ready? ")
        a=a.lower()
        if len(a)>1:
            a=a[0]
        if a=="y":
            check=0
    print ("Begin!")
    time1=time.time() #time start
    for i in range(10): #game function
        score=setup(L,T,used,score)
    time2=time.time() #time end
    print
    tottime=time2-time1
    print ("Alright, that's ten.")
    print ("You got,", score,"right.")
    finale(score,name,tottime) #ending
    

main()
