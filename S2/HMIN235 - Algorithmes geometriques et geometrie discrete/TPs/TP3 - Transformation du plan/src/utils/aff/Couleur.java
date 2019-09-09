package utils.aff;

import java.awt.Color;

public class Couleur {
	public static Color bg = new Color(34, 34, 67); // blue
	public static Color fg = new Color(226, 204, 29);// yellow
	public static Color hl = new Color(250, 67, 167); // magenta
	public static Color other = new Color(51, 211, 204);// cyan

	public static void forPrinter(boolean printer) {
		if (printer) {
			bg = Color.white;
			fg = Color.black;
			hl = Color.black; 
		}
	}
}
