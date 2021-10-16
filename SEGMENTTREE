import java.util.*;

public class Main
{
    static class segtree { // supports queries on abelian group G = (Z, +) where + is a binary operation.
	    
	    static int ceillog(int x) {
	    	int count = 0;
		    int pow = 1;
    		while (pow * 2 <= x) {
    			pow *= 2;
	    		count++;
	    	}
	    	if (pow == x) return count;
    		return count + 1;
    	}
	
	    private int[] arr;
    	private int[] tree;
    	private int n;
    	private int size;
    	
    	public segtree(int[] ray) {
    	    n = ray.length;
    	    size = (int)(Math.pow(2, ceillog(n)));
    	    arr = new int[size];
    	    tree = new int[2 * size]; // 0 1 2 ... 2*size - 1 2*size. 0 and 2*size are unused
    	    
    	    for (int i = 0; i < n; i++) arr[i] = ray[i];
		    for (int i = size; i < 2 * size; i++) tree[i] = arr[i - size]; // range length 1
		
		    for (int i = size - 1; i > 0; i--) {
		    	tree[i] = combine(tree[2 * i], tree[2 * i + 1]);
		    }
    	}
    	
    	public static int combine(int a, int b) { // merge function
    	    return Math.min(a, b);
    	}
	
    	public int query(int a, int b) { // example query
    		a += size;
    		b += size;
    		
	    	int res = Integer.MAX_VALUE; // initial value
	    	while (a <= b) {
    			if (a % 2 == 1) res = combine(res, tree[a++]);
    			if (b % 2 == 0) res = combine(res, tree[b--]);
    			a /= 2;
    			b /= 2;
    		}
    		return res;
    	}
	
    	public int change(int i, int x) { // example update
    		arr[i] += x;
    		i += size;
    		
    		tree[i] += x;
    		
    		for (int k = i / 2; k >= 1; k /= 2) {
    			tree[k] = combine(tree[2 * k], tree[2 * k + 1]);
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

	public static void main(String[] args) {
		Scanner x = new Scanner(System.in);
		int n = x.nextInt();
		int[] arr = new int[n];
		for (int i = 0; i < n; i++) arr[i] = x.nextInt();
		
		segtree tree = new segtree(arr);
		
		System.out.println(tree);
	}
}
