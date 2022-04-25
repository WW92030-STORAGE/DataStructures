import java.util.*;

class Trie {
    class TrieNode {
        HashMap<Character, TrieNode> children;
        boolean end;
        
        TrieNode() {
            end = false;
            children = new HashMap<Character, TrieNode>();
        }
        
        boolean has(char c) {
            return this.children.containsKey(c);
        }
        
        TrieNode get(char c) {
            if (!this.has(c)) return null;
            return this.children.get(c);
        }
        
        TrieNode push(char c) {
            if (!this.has(c)) this.children.put(c, new TrieNode());
            return this.get(c);
        }
    }
    
    TrieNode root;
    
    void insert(String s) {
        int n = s.length();
        TrieNode node = root;
        
        for (int i = 0; i < n; i++) {
            char c = s.charAt(i);
            node = node.push(c);
        }
        node.end = true;
    }
    
    boolean search(String s) {
        int n = s.length();
        TrieNode node = root;
        
        for (int i = 0; i < n; i++) {
            char c = s.charAt(i);
            if (!node.has(c) || node.get(c) == null) return false;
            node = node.push(c);
        }
        return node.end;
    }
    
    static boolean isLeaf(TrieNode n) {
        return n.children.isEmpty();
    }
    
    static TrieNode erase(TrieNode n0, String s, int depth) {
        int n = s.length();
        if (n0 == null) return null;
        if (depth == n) {
            if (n0.end) n0.end = false;
            if (isLeaf(n0)) n0 = null;
            return n0;
        }
        
        char c = s.charAt(depth);
        n0.children.put(c, erase(n0.get(c), s, depth + 1));
        
        if (isLeaf(n0) && !n0.end) n0 = null;
        return n0;
    }
    
    String remove(String s) {
        erase(this.root, s, 0);
        return s;
    }
    
    public Trie() {
        root = new TrieNode();
    }
}

public class TRIE
{
	public static void main(String[] args) {
		Trie trie = new Trie();
		
		String keys[] = {"proto", "protogen", "primagen", "species", "terrain", "terraria", "terra", "terrarium", "prototype", "spectral", "deception", "darkness"};
		for (String s : keys) trie.insert(s);
		
		String queries[] = {"proto", "pro", "prime", "specs", "species", "terrarian", "terra", "spectrum", "deceptive", "protogen", "deceive", "dark"};
		for (String s : queries) System.out.print(trie.search(s) ? s + "\n" : ""); // proto species terra protogen
		
		trie.remove("proto");
		System.out.println();
		for (String s : queries) System.out.print(trie.search(s) ? s + "\n" : ""); // species terra protogen
	}
}




