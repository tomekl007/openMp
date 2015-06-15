package prir.prir3;

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.util.*;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.atomic.AtomicInteger;

public class PMO_Client {

	private final static int PLAYERS = 31;
	public final static int PLAYERS_IN_TEAM = GameInterface.PLAYERS_IN_TEAM;
	private final static AtomicInteger counter = new AtomicInteger(0); // pomocnik
																		// w
																		// liczeniu

	private static GameInterface gi;
	private static CyclicBarrier cb = new CyclicBarrier(PLAYERS,
			new Runnable() {
				public void run() {
					PMO_SOUT.println("--- BARIERA ---");
				}
			});
	private static Set<Long> si = new HashSet<>();
	private static Map<Long, List<Long>> idTeam = new HashMap<>();

	private static final Map<Long, List<GameInterface.Move>> idMoves = Collections.synchronizedMap(new HashMap<Long, List<GameInterface.Move>>());

	private static void sleep(long msec) {
		try {
			Thread.sleep(msec);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	private static String moveToString( GameInterface.Move mv, Long id ) {
        if(mv == null ){
            return "empty move for id !" + id ;
        }
		return "Ruch : uid = " + mv.uid + " phase = " + mv.phase + " code = " + mv.code ; 
	}

	private static class Player implements Runnable {

		private long id; // id przyznane przez system
		private long localID; // lokalny numer ID od 1 do PLAYERS

		private void barrier() {
			try {
				cb.await();
			} catch (InterruptedException | BrokenBarrierException e) {
				e.printStackTrace();
			}
		}

		private boolean movesEqual(GameInterface.Move m1, GameInterface.Move m2) {
            if(m1 == null || m2 == null) return false;

			if (m1.code != m2.code)
				return false;
			if (m1.phase != m2.phase)
				return false;
			if (m1.uid != m2.uid)
				return false;
			return true;
		}

		public Player() {
			localID = counter.incrementAndGet();
		}

		@Override
		public void run() {

			barrier();

			try {
				id = gi.register();
				barrier();
				PMO_SOUT.println( "LokalneID " + localID + " zdalne " + id );
				synchronized (si) {
					si.add(id);
				}
			} catch (RemoteException e) {
				e.printStackTrace();
			}

			if (localID == 1)
				counter.set(0); // jeden z graczy przygotowuje licznik
			barrier();

			if (si.size() != PLAYERS) {
				if (localID == 1)
					PMO_SOUT.printlnErr("Blad na etapie rejestracji - kontynuacja pracy nie ma sensu, graczy"
							+ " jest "
							+ PLAYERS
							+ " a przydzielonych identyfikatorow " + si.size());
				System.exit(1);
			}

			// czekamy na podlaczenie do gry (wygenerowanie teamu)
			// 50x100 msec -> 5 sek maximum
			try {
				int trials = 50;
				do {
					trials--;
					PMO_Client.sleep(100);
				} while ((!gi.isGameReady(id)) && (trials > 0));
			} catch (RemoteException e) {
				e.printStackTrace();
			}

			// zliczamy graczy przydzielnych do gier

			boolean playerInGame = false;

			try {
				if (playerInGame = gi.isGameReady(id)) {
					counter.incrementAndGet();
				}
			} catch (RemoteException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			// czekamy na wszystkich
			barrier();

			int playersInGame = (PMO_Client.PLAYERS / PMO_Client.PLAYERS_IN_TEAM)
					* PMO_Client.PLAYERS_IN_TEAM;
			
			// sprawdzamu poprawnosci podzialu na teamy
			if (counter.get() != playersInGame ) {
				if (localID == 1)
					PMO_SOUT.printlnErr("Blad - gracze zle podzieleni na zespoly - nie kontunuuje pracy, w grze "
							+ counter.get() + " graczy, a powinno byc " + playersInGame );
				System.exit(1);
			}

			if (playerInGame) {
				PMO_SOUT.println("Gracz : " + localID
						+ " przydzielony do gry z id: " + id);

				if (localID == 1) {
					PMO_SOUT.println("Sprawdzamy poprawnosc skladow");
				}
				barrier();

				long[] teamIDs = null;
				try {
					teamIDs = gi.getTeam(id);

					synchronized (idTeam) {
						idTeam.put(id, new ArrayList<Long>());
						for (long i : teamIDs) {
							idTeam.get(id).add(i);
						}
					}

					barrier();

					// sprawdzamy czy moje id jest na listach innych graczy z
					// teamu

					boolean res = true;
					for (long i : teamIDs) {
						if (!idTeam.get(i).contains(id)) {
							PMO_SOUT.printlnErr("Moje ID nie jest na liscie graczy innego gracza z zespolu");
							res = false;
						}
					}

					if (res) {
						PMO_SOUT.println("Test skladow zespolow zaliczony");
					} else {
						PMO_SOUT.printlnErr( "Test budowy zespolow nie zostal zaliczony, blad krytyczny");
						System.exit( 1 );
					}

				} catch (RemoteException e) {
					e.printStackTrace();
				}
                synchronized ( idMoves ) {
                    idMoves.put(id, new ArrayList<GameInterface.Move>());
                }

				// symulacja gry, wykonujemy 3 fazy gry, wysylamy ruch, odbieramy ruch przeciwnikow
				// sprawdzamy czy faktycznie przeciwnicy wyslali takie posuniecia
				for (int ph = 0; ph < 3; ph++) {

					int phase = 0;
					try {
						phase = gi.getPhase(id);
					} catch (RemoteException e) {
						PMO_SOUT.printlnErr("Poprawne wywolanie getPhase spowodowalo wyjatek ");
					}

					if (phase != ph + 1) {
						PMO_SOUT.printlnErr("Bledny numer fazy gry, jest "
								+ phase + " a powinno byc " + (ph + 1));
					}

					GameInterface.Move mv = new GameInterface.Move();
					mv.phase = phase;
					mv.uid = id;
					mv.code = (ph + 1) * 10000 + id;
					
					try {
						gi.move(mv);
						PMO_SOUT.println( "Gracz uid " + id + " wyslal ruch " + moveToString( mv, id ));
					} catch (RemoteException e) {
						PMO_SOUT.printlnErr("Poprawne wywolanie move() spowodowalo wyjatek ");
					}

					// dodaje ruch do rejestru wykonanych posuniec
					synchronized (idMoves) {
						idMoves.get(id).add(mv);
					}

					// czekam na przeciwnikow
					barrier();

					try {
						for (int i = 0; i < PLAYERS_IN_TEAM - 1; i++) {
							mv = gi.getMove(id); // odebrany ruch przeciwnika
							PMO_SOUT.println( "Gracz uid " + id + " odebral ruch " + moveToString( mv, id ));

							boolean found = false;
							outside:
							for (int j = 0; j < PLAYERS_IN_TEAM; j++) { // przegladam
																		// team
								if (teamIDs[j] != id) { // to nie ja
									for (GameInterface.Move mv2 : idMoves
											.get(teamIDs[j])) {
										if (movesEqual(mv, mv2)) {
											found = true;
											break outside;
										}
									}
								}
							}
							if (!found) {
								PMO_SOUT.printlnErr("Odebrany przez uid " + id + " ruch " + moveToString(mv, id) + " nie figuruje na liscie wyslanych posuniec przeciwnikow");
 								if ( mv.uid == id ) {
									PMO_SOUT.printlnErr( "Do gracza wysylane sa jego wlasne posuniecia");
								}
							}
						} // i

						mv = gi.getMove(id); // odebrany ruch przeciwnika, teraz
												// powinno byc NULL

						if (mv != null) {
							PMO_SOUT.printlnErr("Odebrano dla uid " + id + " ruch rozny od null, choc posuniecia powinny juz sie skonczyc. Odebrano " + moveToString( mv, id ));
						}

					} // try
					catch (RemoteException e) {
						PMO_SOUT.printlnErr("Poprawne wywolanie getMove() spowodowalo wyjatek ");
					}

					// spotkane po odebraniu posuniec przeciwnikow
					barrier();

				}

				barrier();

			} else {
				barrier();
				barrier();
				barrier();
				barrier();
				barrier();
				barrier();
				barrier();
				barrier();
				barrier(); // to dla rownowagi -- tak pracuja inni gracze

				PMO_SOUT.println("Gracz : " + localID
						+ " nie zostal przydzielony do gry. Ma on id: " + id);

				PMO_SOUT.println("Wolny gracz rozpoczyna testy systemu.");

				PMO_SOUT.println("Wolny gracz prosi o sklad teamu");
				try {
					gi.getTeam(id);
				} catch (GameInterface.PlayerNotAssignedToGame e) {
					PMO_SOUT.println("Zlapano poprawnie wyjatek : PlayerNotAssignedToGame");
				} catch (Exception e) {
					PMO_SOUT.printlnErr("Mam wyjatek " + e.getMessage());
				}

				PMO_SOUT.println("Wolny gracz prosi o faze gry");
				try {
					gi.getPhase(id);
				} catch (GameInterface.PlayerNotAssignedToGame e) {
					PMO_SOUT.println("Zlapano poprawnie wyjatek : PlayerNotAssignedToGame");
				} catch (Exception e) {
					PMO_SOUT.printlnErr("Mam wyjatek " + e.getMessage());
				}

				PMO_SOUT.println("Wolny gracz wysyla ruch");
				try {
					gi.move(new GameInterface.Move());
				} catch (GameInterface.PlayerNotAssignedToGame e) {
					PMO_SOUT.println("Zlapano poprawnie wyjatek : PlayerNotAssignedToGame");
				} catch (Exception e) {
					PMO_SOUT.printlnErr("Mam wyjatek " + e.getMessage());
				}

			}

		}
	}

	public static void main(String[] args) throws Exception {

		gi = (GameInterface) Naming.lookup("rmi://127.0.0.1/GAME");

		PMO_SOUT.println("MAIN: Generowanie i uruchamianie graczy");
		Thread[] players = new Thread[PLAYERS];
		for (int i = 0; i < PLAYERS; i++) {
			players[i] = new Thread(new Player());
			players[i].setDaemon(true);
			players[i].start();
		}

		PMO_SOUT.println("MAIN: Czekamy na koniec gry");
		for (int i = 0; i < PLAYERS; i++)
			players[i].join();

		System.exit(0);
	}
}
