struct DSU {
    N: usize,
    parent: Vec<usize>,
    size: Vec<usize>,
}


impl DSU {
    fn init(&mut self, n: usize) {
        self.N = n;
        self.parent = vec![0; self.N];
        self.size = vec![1; self.N];
        for i in 0..self.N {
            self.parent[i] = i;
        }
    }
    
    fn find(&self, mut val: usize) -> usize {
        while (self.parent[val] != val) {
            val = self.parent[val];
        }
        return val;
    }
    
    // merges the sets containing (a) and (b) together. returns success status and final root of the merged trees
    fn merge(&mut self, a: usize, b: usize) -> (bool, usize) {
        let r1: usize = self.find(a);
        let r2: usize = self.find(b);
        
        if (r1 == r2) {
            return (false, 0);
        }
        
        let s1: usize = self.size[r1];
        let s2: usize = self.size[r2];
        
        // merge r2 into r1
        if (s1 > s2) {
            self.parent[r2] = r1;
            self.size[r1] += s2;
            return (true, r1);
        } else { // merge r1 into r2
            self.parent[r1] = r2;
            self.size[r2] += s1;
            return (true, r2);
        }
    }
}
