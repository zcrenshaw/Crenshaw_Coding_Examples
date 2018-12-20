package edu.virginia.engine.display;

import javax.swing.*;
import java.awt.*;
import java.io.*;
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
        try (Scanner scanner = new Scanner(file);) {
            scanner.useDelimiter("@");
            int ntoken = 0;
            while (scanner.hasNext()) {
                String token = scanner.next();
                ntoken++;
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

    }

}

