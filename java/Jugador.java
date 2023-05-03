
package clases;

import java.util.concurrent.Semaphore;

public class Jugador extends Thread {

    protected String nombre;
    protected int turn;
    protected int[] results;
    protected Semaphore s1, s2;
    protected int[] winnersByMatch;

    public Jugador(String nombre, int turn, int[] results, Semaphore s1, Semaphore s2, int[] winnersByMatch) {
        this.nombre = nombre;
        this.turn = turn;
        this.results = results;
        this.s1 = s1;
        this.s2 = s2;
        this.winnersByMatch = winnersByMatch;
    }

    public void run() {
        int resul = 0;
        for (int a = 0; a < 5; a++) {
            try {
                s1.acquire();
                resul = (int) (Math.random() * 6 + 1);
                this.results[turn] = resul;
                System.out.println(nombre + "  result " + resul);
                s2.release();
                sleep(1000);
            } catch (InterruptedException e) {
            }

        }
    }

    public static void main(String[] args) {
        int[] buffer = new int[2];
        int[] winnersByMatch = new int[2];
        Semaphore s1 = new Semaphore(1);
        Semaphore s2 = new Semaphore(0);

        Thread jug1 = new Jugador("jugador1", 0, buffer, s1, s2, winnersByMatch);
        Thread jug2 = new Jugador2("jugador2", 1, buffer, s1, s2, winnersByMatch);
        jug1.start();
        jug2.start();
    }

    private static class Jugador2 extends Jugador {

        public Jugador2(String nombre, int turn, int[] results, Semaphore s1, Semaphore s2, int[] winnersByMatch) {
            super(nombre, turn, results, s1, s2, winnersByMatch);
        }

        public void run() {
            int resul = 0;
            for (int a = 0; a < 5; a++) {
                try {
                    s2.acquire();
                    resul = (int) (Math.random() * 6 + 1);
                    this.results[turn] = resul;
                    System.out.println(nombre + "  result " + resul);
                    if (results[0] > results[1]) {
                        winnersByMatch[0]++;
                    } else if ((results[0] < results[1])) {
                        winnersByMatch[1]++;
                    }
                    s1.release();
                    sleep(1000);
                } catch (InterruptedException e) {
                }

            }
            System.out.println("PARTIDAS GANADAS:  JUGADOR1: " + winnersByMatch[0] + "  JUGADOR2: " + winnersByMatch[1]);
            if (winnersByMatch[0] > winnersByMatch[1]) {
                System.out.println("Jugador1 ha ganado");
            } else if (winnersByMatch[0] < winnersByMatch[1]) {
                System.out.println("Jugador2 ha ganado");
            } else {
                System.out.println("Empate ganado");
            }
        }
    }
}
