package main;

import java.awt.BorderLayout;

import javax.swing.BorderFactory;
import javax.swing.JFrame;

import utils.aff.Couleur;
import utils.aff.Vue;

public class Main {

	public static void main(String s[]) {
		javax.swing.SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				int x = 10, y = 10, w = 800, h = 600;
				String fileName = "figures/trio-hypo-2.csv";
				JFrame frame = new JFrame("Transformations dans le plan");
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				Vue v1;
				v1 = new Vue(w, h, fileName, false);
				v1.setBorder(BorderFactory.createLineBorder(Couleur.fg) );
				frame.add(v1);
				frame.pack();
				frame.setLocation(x, y);
				frame.setVisible(true);
			}});
	}
}
