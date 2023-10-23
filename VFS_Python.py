class FileNode: 
  def __init__(self, name, is_directory=False): 
    self.name = name
    self.is_directory = is_directory
    self.children = {} 
    
class FileSystem: 
  def __init__(self): 
    self.root = FileNode("/", True) 
    
  def get_directory(self, path): 
    if(path == "/"): 
      return self.root 
    
    current = self.root
    components = path.split("/")[1:]
    for c in components: 
      if c not in current.children: 
        return None 
      current = current.children[c] 
    return current
  
  def get_parent_directory(self, path): 
    current = self.root
    parent  = None 
    components = path.split("/")[1:]
    for c in components: 
      if c not in current.children:
        return None 
      parent  = current
      current = current.children[c]
    return parent 
  
  def mkdir(self, path): 
    current = self.root
    components = path.split("/")[1:]
    for c in components:  
      if c not in current.children: 
        current.children[c] = FileNode(c, True)
      current = current.children[c]
  
  def rmdir(self, path): 
    parent    = self.get_parent_directory(path)
    nodename  = path.split("/")[-1] 
    
    if parent is None: 
      print("No such directory") 
      return 
    node = parent.children[nodename]
    if node is None: 
      print("No such directory") 
      return
    
    del node 
    del parent.children[nodename]

  
  def dfs(self, lst, current): 
    if current is None: return
  
    lst.append(current.name) 
    print(str(lst)) 
    for v in current.children.values(): 
      self.dfs(lst, v)
    lst.pop()
    return
    
  def ls(self, path): 
    node = self.get_directory(path) 
    if node is None: 
      print("No such directory") 
      return
    for k in node.children.keys(): 
      print(k, end = " ") 
      
  def ls_recur(self, path): 
    node = self.get_directory(path) 
    if node is None: 
      print("No such directory") 
      return 
    self.dfs([], node)  
  
  def add_file(self, path): 
    components = path.split("/")
    filename   = components[-1] 
   
    dir_path   = "/".join(components[:-1])
   
    node = self.get_directory(dir_path) 
    if node is None: 
      print("No such directory") 
      return False
    
    node.children[filename] = FileNode(filename) 
    return True
    

f = FileSystem()
    
f.mkdir("/hello/darkness") 
f.mkdir("/joker/batman/horse") 
f.mkdir("/a/b/c/d/e/f/g") 
f.rmdir("/joker")
f.ls_recur("/") 
