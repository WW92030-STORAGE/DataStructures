import java.util.*;

public class ORDERSTATISTICBINTREE
{
    static class bintree {
	
	    private int[] arr;
    	private int[] tree;
    	private int n;
    	
    	public bintree(int[] ray) {
    	    n = ray.length;
    	    arr = new int[n];
    	    tree = new int[n + 1];
    	    
    	    for (int i = 0; i < n; i++) change(i, ray[i]);
    	}
    	
    	public int psum(int index) {
    	    if (index < 0) return 0;
    	    int res = 0;
    	    index++;
    	    
    	    while (index > 0) {
    	        res += tree[index];
    	        index -= (index & (-1 * index));
    	    }
    	    return res;
    	}
    	
    	public int query(int a, int b) {
    	    return psum(b) - psum(a - 1);
    	}
    	
    	public int change(int i, int x) {
    	    arr[i] += x;
    	    i++;
    	    while (i <= n) {
    	        tree[i] += x;
    	        i += (i & (-1 * i));
    	    }
    	    return x;
    	}
    	
    	public int update(int i, int x) {
    	    int prev = arr[i];
    	    change(i, x - prev);
    	    return x;
    	}
    	
    	public String toString() {
    		String s = "[";
    		for (int i = 0; i < arr.length; i++) s = s + "" + arr[i] + " ";
    		s = s + "] | [";
    		for (int i = 0; i < tree.length; i++) s = s + "" + tree[i] + " ";
    		s = s + ']';
    		return s;
    	}
    }
    
    static class orderedset {
        private bintree tree;
        public orderedset(int size) {
            tree = new bintree(new int[size]);
        }
        
        public int add(int x) {
            tree.update(x, 1);
            return x;
        }
        
        public int remove(int x) {
            tree.update(x, 0);
            return x;
        }
        
        public boolean contains(int x) {
            return tree.arr[x] == 1;
        }
        
        public int index(int x) {
            if (!contains(x)) return -1;
            return tree.psum(x) - 1;
        }
        
        public String toString() {
            int size = tree.n;
            String s = "" + tree.psum(size - 1) + " / " + size + " [ ";
            for (int i = 0; i < size; i++) {
                if (tree.arr[i] == 1) s = s + "" + i + " ";
            }
            s = s + "]";
            return s;
        }
    }

	public static void main(String[] args) {
		Scanner x = new Scanner(System.in);
		
	    orderedset set = new orderedset(1000);
	    for (int i = 0; i < 150; i += 3) set.add(i);
	    for (int i = 0; i < 150; i += 5) set.remove(i);
	    
	    // the set should have all multiples of 3 which are not divisible by 5
	    
	    System.out.println(set);
	    
	    System.out.println("36 is set[" + set.index(36) + "]"); // should be 10th smallest aka. index 9
	}
}









