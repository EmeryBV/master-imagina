package utils.go;

import java.awt.Color;
import java.awt.FontMetrics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Point2D;
import utils.aff.Couleur;

public class PointVisible extends Rectangle {
	private static int midWidth = 5;
	private Color color = Couleur.fg;
	private String label;
	private Point2D.Double P; // coordonnées dans le modèle

	public void setLabel(String label) {
		this.label = label;
	}

	public String toString() {
		return this.label;
	}

	public PointVisible(int x, int y) {
		super(x, y, 2 * PointVisible.midWidth, 2 * PointVisible.midWidth);
		this.P = new Point2D.Double((double) x, (double) y);
		label = "p";
	}

	public void dessine(Graphics2D g2d) {
		g2d.setColor(color);
		g2d.fill(new Ellipse2D.Double(this.x - midWidth, this.y - midWidth, 2 * midWidth, 2 * midWidth));
		drawLabel(g2d);
	}

	public void print() {
		System.out.println("x = " + x + " y = " + y + " w = " + width + " h = " + height);
	}

	public void drawLabel(Graphics2D g) {
		FontMetrics fm = g.getFontMetrics();
		String longLabel;
		int centeredText;
		longLabel = label + "(" + (int)P.x + "," + (int)P.y + ")";
		centeredText = (int) (x - fm.stringWidth(longLabel) / 2);
		g.drawString(longLabel, centeredText, (int) (y - midWidth - fm.getDescent()));
	}

	//return model coordinates for this point
	public Point2D.Double getMC() {
		return P;
	}

	public void copyModelToViewportCoords() {
		x = (int) P.x;
		y = (int) P.y;
		
	}
}
