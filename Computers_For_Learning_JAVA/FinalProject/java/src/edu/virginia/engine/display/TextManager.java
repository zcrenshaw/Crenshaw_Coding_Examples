package edu.virginia.engine.display;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.util.Collection;
import java.util.HashMap;
import java.util.Scanner;

public class TextManager extends JFrame {

    private HashMap texts;
    private int size = 0;


    public TextManager() {

        this.texts = new HashMap();
    }

    public void LoadText(int id, String s) {
        texts.put(id, s);
        size++;

    }

    public void LoadTextfromFile(String filename) {
        File file = new File("resources" + File.separator + filename);
        int id = size;
        String addMe = "";
        try (Scanner scanner = new Scanner(file);) {
            scanner.useDelimiter("@");
            int ntoken = 0;
            while (scanner.hasNext()) {
                addMe = "";
                String token = scanner.next();
                ntoken++;
                //System.out.printf("%3d) %s%n", ntoken, token);

               /* if (token == null) {
                    return;
                }
                System.out.println("TOKEN:");
                System.out.println(token);
                Scanner newLine = new Scanner(token);
                newLine.useDelimiter("\\s+");
                while(newLine.hasNext()){
                    String breakUp = newLine.next();
                   // System.out.println(breakUp);
                 //   System.out.println("NEW ELEMENT");
                    addMe = addMe + "\n\n" + breakUp;
                }
                System.out.println(addMe);
                System.out.println("NEXT:");

*/

                texts.put(id,token);
                id++;
                size++;
            }
        } catch (IOException e) {
                e.printStackTrace();
        }
    }

    public String GetText(int id) {
        return (String)texts.get(id);
    }

    public Collection getValues() {return this.texts.values();}


    public void printText(String s, Graphics g, int x, int y, int size) {
        if (s == null) {
            return;
        }

        y += 10;

        Scanner scanner = new Scanner(s);

        scanner.useDelimiter("\n");
        while(scanner.hasNext()){
            String token = scanner.next();
            g.drawString(token, x, y);
            y += size;
        }



       //g.drawString(s,x,y);


    }

}

