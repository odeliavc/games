import javax.swing.JOptionPane;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;


public class FourInRowController {

    @FXML
    private Pane boardPane;

    @FXML
    private GridPane controlPan;
    
    private Button[] btns;
    private final int COLUMNS = 7; 
    private final int ROWS = 6; 
    private char[][] boardNow = new char [7][8]; // Helper array with the board's current status
    private int[] countTokenRow = new int [8]; // Array which counts tokens per column
    private Button tempBut;
    private Circle newToken;
    private int sizeColumn;
    private int sizeRow;
    private int RadiuSize;
    private char player ;
    private int counterRed = 0,counterBlue = 0;
    private boolean win = false;
    
    public void initialize() {
        sizeRow = (int) (boardPane.getPrefHeight()/ROWS);
        sizeColumn = (int) (boardPane.getPrefWidth()/COLUMNS);
        RadiuSize = sizeRow / 2;
    	handleButtons();
    	creatBoard();
    }
    
    @FXML
    void clearBoard(ActionEvent event) {
    	boardPane.getChildren().clear();
    	creatBoard();
    	for(int i = 1;i <= COLUMNS; i++) {
    		countTokenRow[i] = 0;
    	}
    	for(int i= 0; i <= ROWS; i++) {
    		for(int j=0; j < COLUMNS; j++) {
    			boardNow[i][j] = 'n';
    		}
    	}
    	win = false;
    }
    
    /**
     * Creates the required buttons for the connect-4 board (1 per column)
     */
    private void handleButtons() {
    	btns = new Button[COLUMNS];
		for ( int i = 0; i < COLUMNS ; i++) {
			btns[i] = new Button((i + 1) + "");
			btns[i].setPrefSize(controlPan.getPrefWidth() / COLUMNS, controlPan.getPrefHeight() );
			controlPan.add(btns[i], i , i / COLUMNS);
			
			btns[i].setOnAction(new EventHandler<ActionEvent>() {
				
				@Override
				public void handle(ActionEvent event) {
					if(win == false) {
					 handleButtonsActions(event);
					}
				}
			});
		}
    }
    
    /*
     * Defines the functionality of the buttons created on handleButtons
     */
    private void handleButtonsActions(ActionEvent event) {	
    	tempBut = (Button) event.getSource();
    	int column = Integer.parseInt(tempBut.getText());
    	boardNow[countTokenRow[column]][column - 1] = player;
    	countTokenRow[column]++;
    	if(countTokenRow[column] > 6) {
    		countTokenRow[column]--;
    		Alert alert = new Alert(AlertType.ERROR,"The column is already full");
    		alert.showAndWait();
    		return;
    	}
    	
    	if(player == 'r') {
    		newToken = new Circle(((column - 0.5) * sizeColumn), 
    				boardPane.getPrefHeight() - ((sizeRow * countTokenRow[column]) - sizeRow / 2), RadiuSize, Color.RED);
    		player = 'b';
    	} else {
    		newToken = new Circle (((column - 0.5) * sizeColumn), 
    				boardPane.getPrefHeight() - ((sizeRow * countTokenRow[column]) - sizeRow / 2), RadiuSize, Color.BLUE);
    		player = 'r';
    	}
    	boardPane.getChildren().add(newToken);
    	
    	checkWin();

    }
    
    /*
     * Creates the rows and columns of the table
     */
    private void creatBoard() {
    	player = 'r';
    	int j;
    	Line line;
    	for(j= 0; j < COLUMNS; j++){
    		line = new Line(sizeColumn*j,0 ,sizeColumn*j, boardPane.getPrefHeight());
    		boardPane.getChildren().add(line);	
    	}
    	
    	for(j = 0;j < ROWS; j++){
    		line = new Line(0,sizeRow*j , boardPane.getPrefWidth(),sizeRow*j);
    		boardPane.getChildren().add(line);	
    	}
    }
    private void checkWin() { 
    	int i, j, tempI, tempJ;
    	for(i = 0; i < ROWS; i++) { // Check rows for a win
        	counterRed = 0;
        	counterBlue = 0;
    		for(j = 0; j < COLUMNS; j++) {
    			if (boardNow[i][j] == 'r') {
    				counterRed++;
    				counterBlue=0;
    			} else if (boardNow[i][j]=='b') {
    				counterBlue++;
    				counterRed = 0;
    			} else {
    				counterRed = 0;
    				counterBlue = 0;
    			}
        		if(counterRed == 4)
        			redWin();
        			
        		if(counterBlue == 4)
        			blueWin();
    		}
    	}
    	
    	for(i = 0; i < COLUMNS; i++) { // Check column for a win
        	counterRed = 0;
        	counterBlue = 0;
    		for(j = 0; j < ROWS; j++) {
    			if (boardNow[j][i]=='r') {
    				counterRed++;
    				counterBlue = 0;
    			} else if (boardNow[j][i]=='b') {
    				counterBlue++;
    				counterRed = 0;
    			} else {
    				counterRed = 0;
    				counterBlue = 0;
    			}
        		if(counterRed == 4)
        			redWin();
        		if(counterBlue == 4)
        			blueWin();
    		}	
    	}
    	for(i = 2; i >= 0 ;i--) { // Check upper diagonals from left to right
    		tempI =i;  
    		for(j = 0; tempI < ROWS; j++, tempI++) {
    			if (boardNow[tempI][j] == 'r') {
    				counterRed++;
    				counterBlue = 0; 			
    			} else if (boardNow[tempI][j]=='b') {
    				counterBlue++;
    				counterRed = 0;
    			} else {
    				counterRed = 0;
    				counterBlue = 0;
    			}
    			
        		if(counterRed == 4)
        			redWin();
        		if(counterBlue == 4)
        			blueWin();
    		}
    	}
    	
    	for(j = 0; j < 3; j++) { // Checks lower diagonals from left to right
    		tempJ = j;
    		for(i = 0; tempJ < COLUMNS; tempJ++, i++) {
    			if (boardNow[i][tempJ]=='r') {
    				counterRed++;
    				counterBlue = 0;        			
    			} else if (boardNow[i][tempJ]=='b') {
    				counterBlue++;
    				counterRed = 0;  
    			} else {
    				counterRed = 0;
    				counterBlue = 0;
    			}
    			
        		if(counterRed == 4)
        			redWin();
        		
        		if(counterBlue == 4)
        			blueWin();
    		
    		}
    	}
    	i = 0;
    	j = 0;
    	for(;j < ROWS; j++) { // Checks lower diagonals from right to left
    		for(tempJ = j; tempJ >= 0; tempJ--, i++) {
    			if (boardNow[i][tempJ] == 'r') {
    				counterRed++;
    				counterBlue = 0;	
    			} else if (boardNow[i][tempJ]=='b') {
    				counterBlue++;
    				counterRed = 0;
    			} else {
    				counterRed = 0;
    				counterBlue = 0;
    			}
    			
        		if(counterRed == 4)
        			redWin();
        		
        		if(counterBlue == 4)
        			blueWin();	
    		}
    		
    		i = 0;
    	}
    	
    	for(i = 0; i < ROWS; i++) { // Checks top diagonals from right to left
    		j = ROWS;
    		for(tempI = i; tempI < ROWS; tempI++) {
    			if (boardNow[tempI][j]=='r') {			
    				counterRed++;
    				counterBlue = 0;	
    			} else if (boardNow[tempI][j]=='b') {
    				counterBlue++;
    				counterRed = 0;
    			} else {
    				counterRed = 0;
    				counterBlue = 0;
    			}
    			
        		if(counterRed == 4)
        			redWin();
        		
        		if(counterBlue == 4)
        			blueWin();
        		j--;
    		}
    	}
    }
    
    private void redWin() {
    	JOptionPane.showMessageDialog(null, "RED Win the game");
    	win = true;
    }
    private void blueWin() {
    	JOptionPane.showMessageDialog(null, "BLUE Win the game");
    	win = true;
    }
}
