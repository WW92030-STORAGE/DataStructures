public class EASYHASHMAP {
  static String s = "Protogen are Anthro, cyborg-like creatures. Compared to the Primagen, they are more mammal-like in appearance, sporting a more upright stature, alongside a shorter, rounder visor, that is capable of displaying many emotions. It might possibly have the ability to display two colors at once in a gradient.";
	
  public static void main(String[] args) {
		HashingMap protogen = new HashingMap<String, Integer>();
		String[] arr = s.split(" ");
		
		for (String i : arr) protogen.insert(i, i.length());
		
		System.out.println(protogen);
		
		String[] keys = {"Protogen", "rounder", "colors", "upright", "doesnotexist"};
		for (String i : keys) System.out.println(i + " = " + protogen.get(i));
		
		protogen.remove("colors");
		for (String i : keys) System.out.println(i + " = " + protogen.get(i));
		
		protogen.remove("more");
		protogen.remove("two");
		System.out.println(protogen);
	}
}

class HashingMap<K, V> {
    class Node<K, V> {
        K key;
        V val;
        Node next;
            
        public Node() {
            key = null;
            val = null;
            next = null;
        }
        
        public Node(K k, V v) {
            key = k;
            val = v;
            next = null;
        }
    }
        
    final int buckets = 100003; // 10007 or 100003
    private Node[] map;
        
    public HashingMap() {
        map = new Node[buckets];
        for (int i = 0; i < buckets; i++) map[i] = null;
    }
    
    public int hash(K key) {
        int x = (int)(key.hashCode()) % buckets;
        while (x < 0) x += buckets;
        return x % buckets;
    }
    
    public V insert(K key, V val) {
        int x = hash(key);
        Node node = map[x];
        if (node == null) {
            Node n = new Node(key, val);
            map[x] = n;
            return val;
        }
        
        while (node != null) {
            K test = (K)node.key;
            if (test.equals(key)) {
                node.val = val;
                return val;
            }
            node = node.next;
        }
        
        Node newfront = new Node(key, val);
        newfront.next = map[x];
        map[x] = newfront;
        return val;
    }
    
    public boolean containsKey(K key) {
        int x = hash(key);
        if (map[x] == null) return false;
        Node node = map[x];
        while (node != null) {
            if (key.equals(node.key)) return true;
            node = node.next;
        }
        return false;
    }
    
    public V get(K key) {
        int x = hash(key);
        if (map[x] == null) return null;
        Node node = map[x];
        while (node != null) {
            if (key.equals(node.key)) return (V)node.val;
            node = node.next;
        }
        return null;
    }
    
    public V remove(K key) {
        int x = hash(key);
        
        if (map[x] == null) return null;
        Node node = map[x];
        Node prev = null;
        boolean removal = false;
        while (node != null) {
            if (key.equals(node.key)) {
                removal = true;
                break;
            }
            
            prev = node;
            node = node.next;
        }
        
        if (!removal) return null;
        
        if (prev == null && node.next == null) map[x] = null;
        else if (prev == null && node.next != null) map[x] = node.next;
        else if (node.next == null) prev.next = null;
        else prev.next = node.next;
        
        return (V)node.val;
    }
    
    public String toString() {
        String res = "";
        for (int i = 0; i < buckets; i++) {
            if (map[i] == null) continue;
            Node node = map[i];
            res = res + i + ": ";
            while (node != null) {
                res = res + "[" + node.key + " = " + node.val + "]";
                node = node.next;
            }
            res = res + "\n";
        }
        
        return res.substring(0, res.length() - 1);
    }
}
