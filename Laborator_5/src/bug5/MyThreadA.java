package bug5;
/**
 * @author cristian.chilipirea
 * 
 *         Solve the dead-lock
 */

// lockA, lockB - ambele vor fi blocate in block anterior
	// T0 - *lock(a) lock(b) - nu poate, este detinut de T1 unlock(b) unlock(a)
	// T1 - *lock(b) lock(a) - nu poate, este detinut de T0
	// dead-lock
	// at. cand lucram cu lock-uri consecutive, sa avem grija sa lucram cu ele in aceeasi
	// ordine (unlock in ordine inversa)

public class MyThreadA implements Runnable {

	@Override
	public void run() {
		synchronized (Main.lockA) {
			for (int i = 0; i < Main.N; i++)
				Main.valueA++;
		}
			synchronized (Main.lockB) {
				for (int i = 0; i < Main.N; i++)
					Main.valueB++;
			}
	}
}
