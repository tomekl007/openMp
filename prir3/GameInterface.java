package prir.prir3;

import java.io.Serializable;
import java.rmi.Remote;
import java.rmi.RemoteException;

/**
 * Uproszczony interfejs systemu turowej gry sieciowej. 
 *
 */
public interface GameInterface extends Remote {

	/**
	 * Liczba uczestnikow jednej gry. W trakcie testow ta
	 * wartosc moze ulec zmianie.
	 */
	public final int PLAYERS_IN_TEAM = 10;
	
	public class Move implements Serializable {
		private static final long serialVersionUID = -4716482352286627434L;
		public long uid;
		public long code;

        @Override
        public String toString() {
            return "Move{" +
                    "uid=" + uid +
                    ", code=" + code +
                    ", phase=" + phase +
                    '}';
        }

        public int phase;
	}
		
	/**
	 * Wyjatek zglaszany gdy gracz nie jest przypisany do zadnej z gier, a 
	 * zachowuje sie jakby w grze uczestniczyl.
	 */
	public class PlayerNotAssignedToGame extends RemoteException {
		private static final long serialVersionUID = 5070518523926262194L;
	}

	/**
	 * Wyjatek zglaszany gdy gracz probuje przeslac kolejny ruch w tej samej
	 * fazie gry.
	 */
	public class MoveAlreadyDone extends RemoteException {
		private static final long serialVersionUID = 5654536099352858489L;
	}

	/**
	 * Metoda rejestruje gracza w systemie. Gracz uzyskuje unikalny identyfikator, ktorego
	 * nastepnie uzywa w kontakcie z systemem.
	 * 
	 * @return unikalny identyfikator gracza
	 * @throws java.rmi.RemoteException
	 */
	long register() throws RemoteException;
	
	/**
	 * Metoda informuje gracza czy zostal przypisany do gry.
	 * 
	 * @param uid identyfikator gracza
	 * @return true - gracz uczestniczy w grze, false - gracz oczekuje na przypisanie do gry
	 * @throws java.rmi.RemoteException
	 */
	boolean isGameReady(long uid) throws RemoteException ;
	
	/**
	 * Metoda zwraca identyfikatory graczy uczestniczacych w tej samej rozgrywce.
	 * 
	 * @param uid identyfikator gracza
	 * @return tablica identyfikatorow graczy przypisanych do tej samej gry.
	 * @throws java.rmi.RemoteException
	 * @throws prir.prir3.GameInterface.PlayerNotAssignedToGame
	 */
	long[] getTeam(long uid) throws RemoteException, PlayerNotAssignedToGame;
	
	/** 
	 * Metoda pozwala graczowi przeslac informacje o swoim ruchu. W jednej fazie gry wolno
	 * wykonac graczowi jeden ruch.
	 * 
	 * @param mv obiekt reprezentujacy ruch gracza
	 * @throws java.rmi.RemoteException
	 * @throws prir.prir3.GameInterface.MoveAlreadyDone
	 * @throws prir.prir3.GameInterface.PlayerNotAssignedToGame
	 */
	void move(Move mv) throws RemoteException, MoveAlreadyDone, PlayerNotAssignedToGame;
	
	/**
	 * Metoda pozwala na pobranie ruchow wykonanych przez innych graczy. Gracz wykonuje metode
	 * wielokrotnie aby poznac zagrania innych graczy. Gracz nie ma obowiazku wykonywac tej metody
	 * (moze grac "w ciemno"), ale system nie moze zgubic ruchow, ktore gracz do tej pory nie pobral.
	 * Informacja o ruchach udostepniana jest w kolejnosci ich wykonania (najpierw udostepniane sa
	 * informacje o ruchach wykonanych najdawniej, a nastepnie o ruchach nowszych).
	 * 
	 * @param uid identyfikator gracza
	 * @return obiekt reprezentujacy ruch wykonany przez innego gracza, null jesli brak ruchow do pobrania
	 * 
	 * @throws java.rmi.RemoteException
	 * @throws prir.prir3.GameInterface.PlayerNotAssignedToGame
	 */
	Move getMove(long uid) throws RemoteException, PlayerNotAssignedToGame;
	
	/**
	 * Metoda udostepnia informacje o numerze fazy gry. Gra zaczyna sie w fazie numer 1.
	 * Numer fazy ulega inkrementacji, gdy wszyscy gracze wykonaja w danej fazie ruch.
	 * @param uid identyfikator gracza
	 * @return Aktualna faza gry
	 * @throws java.rmi.RemoteException
	 * @throws prir.prir3.GameInterface.PlayerNotAssignedToGame
	 */
	int getPhase(long uid) throws RemoteException, PlayerNotAssignedToGame;
}
