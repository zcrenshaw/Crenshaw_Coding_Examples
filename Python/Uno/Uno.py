#Zack Crenshaw
#Uno simulator
#2/13/15

import random
import time

def intro():
    name=raw_input("Hello there. What's your name? ")
    print "Hello,", name, ". Welcome to 5-card Uno"
    instruct=raw_input("Do you know how to play Uno? ")
    instruct=instruct.lower()
    while instruct=="":
        instruct=raw_input("Do you know how to play Uno? ")
    instruct=instruct[0]
    if instruct=="y":
        print "Great!"
    else:
        print "OK, well here's how: "
        print
        instructions()
    raw_input("Press return to begin ")

def instructions():
    print "In this game, you have to put down cards that either match the"
    print "Color or number of the preceding card."
    print "If you can't put a card down, you draw a card."
    raw_input("Press return to continue")
    print
    print "You can play 'wild' cards no matter what color or number the preceding card is."
    print "With a wild, you'll be able to change the color to one that you want."
    print "If it is a 'wild draw 4', the next person will draw 4 cards as well."
    raw_input()
    print "After a wild is played, you have to play the color chosen (it doesn't matter the number)."
    print "This will be represented with 'any' in the number spot."
    raw_input()
    print "Skip cards will skip the next player."
    raw_input()
    print "Reverse cards will reverse the direction of play."
    raw_input()
    print "Draw 2 cards make the next person draw 2 cards."
    raw_input()
    print "When you or another player has only 1 card left, you have to type 'uno' as fast as you can."
    print "Be ready when you or someone else has 2 cards left."
    raw_input()
    print "You will play with 2 computer players."
    print "Be the first player to lose all their cards."
    print "Good luck!"
    

def loadcards(x):
    for i in range(5):
        x=draw(x)
    return x

def draw(x):
    dc=random.choice(colors)
    if dc=="wild":
        dn=random.choice(specialnumbers)
    else:
        dn=random.choice(numbers)
    x.append((dc,dn))
    return x

def endgame(cycle,restart):
    if cycle==1:
        print "You won!"
    else:
        print "Player", cycle,"has won!"
    restart=raw_input("Would you like to play again? Y/N ")
    while restart=="":
        restart=raw_input("Would you like to play again? Y/N ")
    restart=restart.lower()
    restart=restart[0]
    if restart=="y":
        intro=raw_input("Would you like to see the instructions again? Y/N")
        intro=intro.lower()
        intro=intro[0]
    if intro=="y":
        instructions()
    return restart

def uno(handc, cycle):
    if cycle==1:
        print "QUICK! You have 5 seconds to type 'uno'"
        print "or you DRAW 2 CARDS!"
        unocheck=1
        time1=time.time()
        uno=raw_input("Type now! ")
        while unocheck==1:
            uno=uno.lower()
            if uno=="uno":
                unocheck=0
            else:
                uno=raw_input("Try again! ")
        time2=time.time()
        if time2-time1>5:
            print "Darn. You took a little to long."
            for i in range(2):
                handc=draw(handc)
            print "You drew 2 cards."
            print
        else:
            print "You did it!"
            print
    else:
        print "QUICK! You have 5 seconds to type 'uno'"
        print "to make Player", cycle, "DRAW 2 CARDS!"
        unocheck=1
        time1=time.time()
        uno=raw_input("Type now! ")
        while unocheck==1:
            uno=uno.lower()
            if uno=="uno":
                unocheck=0
            else:
                uno=raw_input("Try again! ")
        time2=time.time()
        if time2-time1<5:
            print "You did it!"
            for i in range(2):
                handc=draw(handc)
            print "Player", cycle, "drew 2 cards."
            print
        else:
            print "Darn. You took a little to long."
            print
    return handc


def AI(pile, cycle, handc):
    valid=1
    tries=0
    print
    print "Player", cycle, " is up!"
    if pile[1]=="draw 4":
        pile[1]="any"
    print "The card to match: ", pile[0], pile[1]
    print
    while valid==1: #make this into for loop to cycle through cards
        cihc=len(handc)
        choice=random.randint(0,cihc+1)
        if choice<=cihc:
            choice=choice-1
            thecard=handc[choice]
            if thecard[0]==pile[0]:
                print "They put down:", thecard[0], thecard[1]
                handc.pop(choice)
                valid=0
                pile[0]=thecard[0]
                pile[1]=thecard[1]
                action=1
            elif thecard[0]=="wild":
                print "They put down:", thecard[0], thecard[1]
                handc.pop(choice)
                valid=0
                pile[0]=random.choice(ncolors)
                pile[1]=thecard[1]
                print "New Color!:", pile[0]
                action=1
            elif thecard[1]==pile[1]:
                print "They put down:", thecard[0], thecard[1]
                handc.pop(choice)
                valid=0
                pile[0]=thecard[0]
                pile[1]=thecard[1]
                action=1
            else:
                valid=1
                tries=tries+1
        if tries>len(handc): #here try to come up with way to cycle through each card
            valid=2
        if valid==2:
            handc=draw(handc)
            valid=0
            print "They drew a card."
    print "Player", cycle," has", len(handc),"cards."
    return pile
    return cycle
    return handc

def main(): 
    intro()
    global action 
    global colors
    colors=['red', 'green', 'blue', 'yellow', "wild"]
    global ncolors
    ncolors=['red', 'green', 'blue', 'yellow']
    global numbers
    numbers = ['0','1','2','3','4','5','6','7','8','9', "draw 2", "skip", "reverse"]
    startnumbers = ['0','1','2','3','4','5','6','7','8','9']
    global specialnumbers
    specialnumbers=["any", "draw 4"]
    restart="y"
    while restart=="y":
        action=0
        pile=[]
        hand=[]
        hand2=[]
        hand3=[]
        rc=random.choice(ncolors)
        rn=random.choice(startnumbers) #better way?
        pile.append(rc)
        pile.append(rn)
        hand=loadcards(hand)
        hand2=loadcards(hand2)
        hand3=loadcards(hand3)
        game=1
        cycle=1
        move=1
        while game==1:
            while cycle==1:
                check=len(hand)
                if action==1:
                    valid=0
                    if pile[1]=="draw 2":
                        for i in range(2):
                           hand=draw(hand)
                        print "You drew 2 cards."
                        action=0
                    elif pile[1]=="draw 4":
                        for i in range(4):
                           hand=draw(hand)
                        print "You drew 4 cards."
                        action=0
                    elif pile[1]=="skip":
                        print "You were skipped."
                        action=0
                    else:
                        valid=1
                else:
                    valid=1
                if pile[1]=="draw 4":
                    pile[1]="any"
                while valid==1:
                    print
                    print "You're up!"
                    print "Your hand:"
                    for i in range(len(hand)):
                        card=hand[i]
                        print i+1,"",card[0], card[1]
                    print "The card to match: ",pile[0], pile[1]
                    print
                    print "Choose a card from your hand: (the number in the set) "
                    choice=raw_input("Or type 'd' to draw a card: ")
                    valid=2
                    if choice.isdigit():
                        choice=eval(choice)
                        if choice<=len(hand):
                            choice=choice-1
                            thecard=hand[choice]
                            print "You chose:",thecard[0], thecard[1]
                            if thecard[0]==pile[0]: #colors
                                hand.pop(choice)
                                valid=0
                                pile[0]=thecard[0]
                                pile[1]=thecard[1]
                                action=1
                            elif thecard[1]==pile[1]:#numbers
                                hand.pop(choice)
                                valid=0
                                pile[0]=thecard[0]
                                pile[1]=thecard[1]
                                action=1
                            elif thecard[0]=="wild": #wilds
                                hand.pop(choice)
                                valid=0
                                wildcheck=1
                                if len(hand)==0:
                                    break
                                else:
                                    pile[0]=raw_input("Choose a color: ")
                                    while wildcheck==1:
                                        pile[0]=pile[0].lower()
                                        if pile[0] not in ncolors:
                                            pile[0]=raw_input("Please input a valid color: ")
                                        else:
                                            wildcheck=0
                                pile[1]=thecard[1]
                                action=1
                            else:
                                print "Try a different card"
                                valid=1
                        else:
                            print "Please input a valid number"
                            valid=1
                    elif choice=="":
                        print "Please input a valid choice."
                        valid=1
                    else: #draw
                        choice=choice.lower()
                        if choice[0]=="d":
                            hand=draw(hand)
                            drawn=hand[-1]
                            print "You drew a card:", drawn[0], drawn[1]
                            valid=0
                        else:
                            print"Please input a valid choice"
                            valid=1
                if len(hand)==0:
                    restart=endgame(cycle,restart)
                    cycle=0
                    game=0
                    move=0
                    break
                if len(hand)==1: #uno function
                    print "You have one card left."
                    hand=uno(hand, cycle)
                if len(hand)==check-1:
                    action=1
                if pile[1]=="reverse":
                    if action==1:
                        move=move*-1
                        print "You reversed the direction!"
                        action=0
                if move==1:
                    cycle=2
                else:
                    cycle=3
                raw_input("Press return to continue. ")
            if cycle==2:
                check=len(hand2)
                if len(hand2)>0:
                    if action==1:
                        if pile[1]=="draw 2":
                            for i in range(2):
                                hand2=draw(hand2)
                            print "Player 2 drew 2 cards."
                            action=0
                        elif pile[1]=="draw 4":
                            for i in range(4):
                                hand2=draw(hand2)
                            print "Player 2 drew 4 cards."
                            action=0
                        elif pile[1]=="skip":
                            print "Player 2 was skipped."
                            action=0
                        else:
                            pile=AI(pile,cycle, hand2)
                            action=0
                    else:
                        pile=AI(pile,cycle, hand2)
                    if len(hand2)==1: #uno function
                        hand2=uno(hand2, cycle)
                    if len(hand2)==check-1:
                        action=1
                    else:
                        action==0
                    if action==1:
                        if pile[1]=="reverse":
                            move=move*-1
                            print "Player 2 reversed the direction."
                            action=0
                        else:
                            none=1
                    else:
                        none=1
                    if move==1:
                        cycle=3
                    else:
                        cycle=1
                    raw_input("Press return to continue. ")
                else:
                    restart=endgame(cycle,restart)
                    cycle=0
                    game=0
                    move=0
                    break
            if cycle==3:
                check=len(hand3)
                if len(hand3)>0:
                    if action==1:
                        if pile[1]=="draw 2":
                            for i in range(2):
                                hand3=draw(hand3)
                            print "Player 3 drew 2 cards."
                            action=0
                        elif pile[1]=="draw 4":
                            for i in range(4):
                                hand3=draw(hand3)
                            print "Player 3 drew 4 cards."
                            action=0
                        elif pile[1]=="skip":
                            print "Player 3 was skipped."
                            action=0
                        else:
                            pile=AI(pile,cycle, hand3)
                            action=0
                    else:
                        pile=AI(pile,cycle, hand3)
                    if len(hand3)==1: #uno function
                        hand3=uno(hand3, cycle)
                    if len(hand3)==check-1:
                        action=1
                    if action==1:
                        if pile[1]=="reverse":
                            move=move*-1
                            print "Player 3 reversed the direction."
                            action=0
                        else:
                            none=1
                    else:
                        none=1
                    if move==1:
                        cycle=1
                    else:
                        cycle=2
                    raw_input("Press return to continue. ")
                else:
                    restart=endgame(cycle,restart)
                    cycle=0
                    game=0
                    move=0
                    break
            
            
    


main()
