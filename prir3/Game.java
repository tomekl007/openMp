package prir.prir3;

import java.io.Serializable;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

 class Game implements GameInterface, Serializable {
    private int platerId = 0;
    private final LinkedList<SpecificGame> specificGames = new LinkedList<>();
    private final Lock specificGameRegisterLock = new ReentrantLock();


    @Override
    public long register() throws RemoteException {
        specificGameRegisterLock.lock();
        try {
            long id = ++platerId;
            boolean joined = false;

            for (SpecificGame specificGame : specificGames) {
                if (couldJoinToGame(specificGame)) {
                    joined = true;
                    specificGame.addPlayer(id);
                }
            }
            if (!joined) {
                SpecificGame newSpecificGame = new SpecificGame();
                newSpecificGame.addPlayer(id);
                specificGames.add(newSpecificGame);
            }
            return id;
        } finally {
            specificGameRegisterLock.unlock();
        }
    }

    private boolean couldJoinToGame(SpecificGame specificGame) {
        return specificGame.players.size() < GameInterface.PLAYERS_IN_TEAM;
    }


    @Override
    public boolean isGameReady(long uid) throws RemoteException {
        specificGameRegisterLock.lock();
        try {
            for (SpecificGame specificGame : specificGames) {
                for (Player player : specificGame.players) {
                    if (player.playerId == uid && specificGame.players.size() == PLAYERS_IN_TEAM) {
                        return true;
                    }
                }
            }
            return false;

        } finally {
            specificGameRegisterLock.unlock();
        }

    }


    @Override
    public long[] getTeam(long uid) throws RemoteException, PlayerNotAssignedToGame {
        for (SpecificGame specificGame : specificGames) {
            for (Player player : specificGame.players) {
                if (player.playerId == uid) {
                    long[] uids = new long[specificGame.players.size()];
                    for (int i = 0; i < specificGame.players.size(); i++) {
                        uids[i] = specificGame.players.get(i).playerId;
                    }
                    return uids;
                }
            }
        }
        throw new PlayerNotAssignedToGame();
    }


    @Override
    public void move(Move mv) throws RemoteException, MoveAlreadyDone, PlayerNotAssignedToGame {
        getGameForPlayerId(mv.uid).move(mv);
    }

    @Override
    public Move getMove(long uid) throws RemoteException, PlayerNotAssignedToGame {
        return getGameForPlayerId(uid).getMove(uid);
    }

    @Override
    public int getPhase(long uid) throws RemoteException, PlayerNotAssignedToGame {
        return getGameForPlayerId(uid).getPhase();
    }

    private SpecificGame getGameForPlayerId(long uid) throws PlayerNotAssignedToGame {
        specificGameRegisterLock.lock();
        try {

            for (SpecificGame t : specificGames) {
                for (Player player : t.players) {
                    if (player.playerId == uid)
                        return t;
                }
            }
            throw new PlayerNotAssignedToGame();
        } finally {
            specificGameRegisterLock.unlock();
        }
    }


    private class SpecificGame {
        private AtomicInteger phase;
        private final LinkedList<Move> moves;
        public final LinkedList<Player> players;

        SpecificGame() {
            phase = new AtomicInteger(1);
            moves = new LinkedList<Move>();
            players = new LinkedList<>();
        }

        public void addPlayer(long uid) {
            players.add(new Player(moves, uid));
        }


        public int getPhase() {
            return phase.get();
        }

        public int size() {
            return players.size();

        }

        public void move(Move mv) throws MoveAlreadyDone {
            synchronized (moves) {
                if (moveForThatPhaseWaAlreadyDone(mv.uid, moves))
                    throw new MoveAlreadyDone();
                moves.add(mv);

                if (allPlayersMovedInGivenPhase(mv)) {
                    phase.incrementAndGet();
                }
            }
        }


        private boolean allPlayersMovedInGivenPhase(Move mv) {

            int numberOfIterations = PLAYERS_IN_TEAM;
            if (this.moves.size() == 0)
                return false;
            else if (this.moves.size() >= PLAYERS_IN_TEAM) {
                Iterator<Move> playerIterator = moves.descendingIterator();
                while (playerIterator.hasNext() && numberOfIterations > 0) {
                    Move next = playerIterator.next();
                    if (next.phase != mv.phase) {
                        return false;
                    }
                    numberOfIterations--;
                }
                return true;
            }else {
                return false;
            }

        }


        public Move getMove(long uid) throws PlayerNotAssignedToGame {
            Player player = null;
            for (Player p : players) {
                if (p.playerId == uid) {
                    player = p;
                }
            }

            if (player == null)
                throw new PlayerNotAssignedToGame();
            return getMoveForPlayer(player, uid);
        }

        private Move getMoveForPlayer(Player player, long uid) {
            Move mv = player.getMove();
            if (mv == null)
                return null;
            if (mv.uid != uid)
                return mv;
            else
                return getMoveForPlayer(player, uid);
        }

        private boolean moveForThatPhaseWaAlreadyDone(long uid, LinkedList<Move> moves) {
         synchronized (this.moves) {
                if (this.moves.size() == 0) {
                    return false;
                }else{
                        Move mv = this.moves.getLast();
                        if (mv.phase != getPhase())
                            return false;
                        if (mv.uid == uid)
                            return true;
                    }
                }
                return false;
            }
    }

    private class Player {
        private AtomicInteger historyId;
        private LinkedList<Move> moves;
        public final long playerId;

        Player(LinkedList<Move> moves, long playerId) {
            this.playerId = playerId;
            historyId = new AtomicInteger(0);
            this.moves = moves;
        }

        public Move getMove() {
            if (historyId.get() >= moves.size() || moves.size() == 0)
                return null;
            return moves.get(historyId.getAndIncrement());
        }
    }
}


class Start {
    public static void main(String[] arg) throws Exception {
        GameInterface rmiService = (GameInterface) UnicastRemoteObject
                .exportObject(new Game(), 0);
        Registry registry = LocateRegistry.getRegistry();
        registry.rebind("GAME", rmiService);
    }
}