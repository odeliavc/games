import java.util.Random;

import javax.swing.JOptionPane;

import javafx.event.ActionEvent;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.event.EventTarget;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.input.DragEvent;
import javafx.scene.layout.GridPane;

public class HitController {

    @FXML
    private GridPane grid;
    @FXML
    private Button flagBut;
    
    private Button[][] btns;
    
    private boolean[][] booms;
    private boolean[][] hits;
    private int[][] helpNum;
    private int counter =0;
    private boolean flag = false;
    
    private Random rand = new Random();
    
    private int numOfBooms = 10;
    
    private int SIZE = 10; 
    
    private int countBooms =0;
    
    
    public void initialize() {
    	getSizeAndNumOfBooms();
    	btns = new Button[SIZE][SIZE];
    	hits = new boolean[SIZE][SIZE]; 
    	booms = new boolean[SIZE][SIZE]; 
		getBooms();
		for (int i =0 ; i<SIZE ; i++) {
			for (int j=0 ; j< SIZE ; j++) {
				btns[i][j] = new Button();
				btns[i][j].setPrefSize(grid.getPrefWidth()/SIZE,grid.getPrefHeight()/SIZE );
				btns[i][j].setOnAction(new EventHandler <ActionEvent>() {
					public void handle(ActionEvent e) {
						handleButton(e,flag);
					}
				});
				
				grid.add(btns[i][j], i % SIZE,j % SIZE);
			}
		}

    }
    @FXML
    void flag(ActionEvent event) {
    	if (flag == false) {
    		flag = true;
    		flagBut.setText("Flag off");
    	}
    	else if (flag == true) {
    		flag = false;
    		flagBut.setText("Flag on");
    	}
    }
    private void getBooms() {
    	int num;
    	helpNum = new int[SIZE][SIZE];
    	countBooms=0;
		for (int i =0 ; i<SIZE ; i++) {
			if (countBooms == numOfBooms) break;
			for (int j=0 ; j< SIZE ; j++) {
				hits[i][j] = false;
				num = rand.nextInt(SIZE*SIZE/numOfBooms);
				helpNum[i][j] = num;
				if (num == 9) countBooms++; 
				if (countBooms == numOfBooms) break;
			}
		}
		if (countBooms < numOfBooms ) {
			int addMoreBooms = numOfBooms - countBooms;
			while (addMoreBooms > 0) {
				int i = rand.nextInt(SIZE);
				int j = rand.nextInt(SIZE);
				if (helpNum[i][j] != 9) {
					helpNum[i][j] = 9 ;
					addMoreBooms--;
				}
			}
		}
		for (int i =0 ; i<SIZE ; i++) {
			for (int j=0 ; j< SIZE ; j++) {
				if (helpNum[i][j] == 9) {
					booms[i][j] = true;
				}
				else {
					booms[i][j] = false;
					helpNum[i][j] = 0;
					try {
						if (helpNum[i-1][j] == 9) helpNum[i][j]++;
					}catch (Exception IndexOutOfBoundsException) {}
					try {
						if (helpNum[i-1][j-1] == 9) helpNum[i][j]++;
					}catch (Exception IndexOutOfBoundsException) {}
					try {
						if (helpNum[i-1][j+1] == 9) helpNum[i][j]++;
					}catch (Exception IndexOutOfBoundsException) {}
					try {
						if (helpNum[i][j+1] == 9) helpNum[i][j]++;
						}catch (Exception IndexOutOfBoundsException) {}
					try {
						if (helpNum[i][j-1] == 9) helpNum[i][j]++;
						}catch (Exception IndexOutOfBoundsException) {}	
					try {
						if (helpNum[i+1][j-1] == 9) helpNum[i][j]++;
						}catch (Exception IndexOutOfBoundsException) {}		
					try {
						if (helpNum[i+1][j] == 9) helpNum[i][j]++;
						}catch (Exception IndexOutOfBoundsException) {}		
					try {
						if (helpNum[i+1][j+1] == 9) helpNum[i][j]++;
						}catch (Exception IndexOutOfBoundsException) {}		
			
				}
			}
		}	
		
    }
    private void handleButton(ActionEvent e,boolean flag) {
		for (int i =0 ; i<SIZE ; i++) {
			for (int j=0 ; j< SIZE ; j++) {
				if (btns[i][j] == e.getSource()) {
					if (flag == false) {
						if (hits[i][j] == true ) {
							return;
						}
						if (booms[i][j] == true) {
							loosing();
						}
						btns[i][j].setText(helpNum[i][j]+"");
						hits[i][j] = true;
						counter++;
						System.out.println(counter);
						if (counter == SIZE*SIZE - numOfBooms) {
						wining();
						}
						if (helpNum[i][j] == 0) {
							itsaZero(i,j);
						}
					}
					if (flag == true) {
						btns[i][j].setText("Boom");
					}
				}
			}
		}
		
    }
    
    private void loosing() {
    	Alert.AlertType type = Alert.AlertType.ERROR;
    	String str = "you are a losser";
    	Alert a = new Alert(type);
    	a.setContentText(str);
    	a.showAndWait();
    }
 
    private void wining() {
    	Alert.AlertType type = Alert.AlertType.CONFIRMATION;
    	String str = "you are a wineer !!!";
    	Alert a = new Alert(type);
    	a.setContentText(str);
    	a.showAndWait();
    }
   
    @FXML
    void reStart(ActionEvent event) {
    	counter =0;
		for (int i =0 ; i<SIZE ; i++) {
			for (int j=0 ; j< SIZE ; j++) {
				btns[i][j].setText("");
			}
		}
		getBooms();
    }
    private void getSizeAndNumOfBooms(){
    	String message = "Enter the size of your board:";
    	SIZE = Integer.parseInt(JOptionPane.showInputDialog(message));
    	message = "Enter the num of Booms that you want:";
    	numOfBooms = Integer.parseInt(JOptionPane.showInputDialog(message));
    }
    
    private void itsaZero(int i,int j){
    	ActionEvent e;
    	System.out.print("lllllaaaaa");
    	try {
    		e = new ActionEvent(btns[i-1][j],null);
    		handleButton(e,flag);
		}catch (Exception IndexOutOfBoundsException) {
			System.out.print("blabla");
		}
		try {
			e = new ActionEvent(btns[i-1][j-1],null); 
    		handleButton(e,flag);
		}catch (Exception IndexOutOfBoundsException) {}
		try {
			e = new ActionEvent(btns[i-1][j+1],null); 
    		handleButton(e,flag);
		}catch (Exception IndexOutOfBoundsException) {}
		try {
			e = new ActionEvent(btns[i][j+1],null); 
    		handleButton(e,flag);
			}catch (Exception IndexOutOfBoundsException) {}
		try {
			e = new ActionEvent(btns[i][j-1],null); 
    		handleButton(e,flag);
			}catch (Exception IndexOutOfBoundsException) {}	
		try {
			e = new ActionEvent(btns[i+1][j-1],null);  
    		handleButton(e,flag);
			}catch (Exception IndexOutOfBoundsException) {}		
		try {
			e = new ActionEvent(btns[i+1][j],null); 
    		handleButton(e,flag);
			}catch (Exception IndexOutOfBoundsException) {}		
		try {
			e = new ActionEvent(btns[i+1][j+1],null); 
    		handleButton(e,flag);
			}catch (Exception IndexOutOfBoundsException) {}		

    }
}
