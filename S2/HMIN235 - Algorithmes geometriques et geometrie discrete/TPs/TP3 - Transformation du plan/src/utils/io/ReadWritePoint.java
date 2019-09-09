package utils.io;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Scanner;

import utils.go.PointVisible;


/** Assumes UTF-8 encoding. JDK 7+. */
public class ReadWritePoint {
	File rf;
	ArrayList<String> textToWrite;
	private final static Charset ENCODING = StandardCharsets.UTF_8;

	public ReadWritePoint(String aFileName) {
		rf = new File(aFileName);
		textToWrite = new ArrayList<String>();
	}

	public ArrayList<PointVisible> read()  {
		ArrayList<PointVisible> points = new ArrayList<PointVisible>();
		try (Scanner scanner = new Scanner(rf, ENCODING.name())) {
			int i = 0;
			while (scanner.hasNextLine()) {
				points.add(readLine(scanner.nextLine(), i++));
			}
			//System.out.println(points.size() + " points lus");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		return points;
	}

	// Suppose que les points sont d�crits ligne par ligne et pour chaque ligne, le format est: 
	// x;y;label
	// x,y sont les coordonn�es et label est l'�tiquette associ�e au point
	PointVisible readLine(String aLine, int i) {
		Scanner scanner = new Scanner(aLine);
		scanner.useDelimiter(";");
		PointVisible p = null;
		String x,y, label;
		
		if (scanner.hasNext()) {
			// assumes the line has a certain structure
			x = scanner.next();
			y = scanner.next();
			label = scanner.hasNext()? scanner.next():"p"+i;

			p = new PointVisible(Integer.parseInt(x), Integer.parseInt(y));
			p.setLabel(label);
		}
		scanner.close();
		return p;
	}

	public void write() {
		PrintWriter pw;
		try {
			pw = new PrintWriter(rf);
			for (String s : textToWrite) {
				pw.println(s);
				pw.flush();
			}
			pw.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}


	public void add(String s) {
		textToWrite.add(s);
	}

}
