#include <iostream> 
#include <unordered_map> 
#include <string> 
#include <vector> 
#include <sstream> 

using namespace std; 

vector<string> split(const string& input, char delimiter) 
{
    vector<string> result;
    istringstream ss(input);
    string token;

    while (getline(ss, token, delimiter)) 
    {
        result.push_back(token);
    }
    return result;
}


class FileNode 
{
    public: 
    string name;
    bool isDirectory;
    unordered_map<string, FileNode*> subdirs;
    
    FileNode(const string& name, bool isDirectory)
    {
        this->name = name;
        this->isDirectory = isDirectory;
    }
};

class FileSystem
{
    private: 
    
    FileNode* root;
    FileNode* getDirNode(const string&);
    void dlt_recur(FileNode*);
    void dfs(FileNode*, vector<string>&);
    
    public:
    
    FileSystem()
    {
       this->root = new FileNode("root", true);
    }
    bool mkdir(const string&); 
    bool touch(const string&);
    bool rmdir(const string&);
    bool mv(const string&, const string&);
    bool ls(const string&);
    bool recur_ls(const string&);
    
    
    ~FileSystem()
    {
        this->dlt_recur(this->root); 
    }
};

FileNode* FileSystem::getDirNode(const string& path)
{
    vector<string> v = split(path, '/');
    int size = (int) v.size();
    
    FileNode* current = this->root;
    
    for(int i = 1; i < size; ++i)
    {
        if((current->subdirs).count(v[i]) == false)
        {
            return NULL;
        }
        current = (current->subdirs)[v[i]];
    }
    
    return current;
}
void FileSystem::dlt_recur(FileNode* current)
{
    if(!current) return; 
    
    unordered_map<string, FileNode*>::iterator it = (current->subdirs).begin(), e = (current->subdirs).end();
    
    while(it != e)
    {
        this->dlt_recur(it->second);
        it++;
    }
    delete current;
}

bool FileSystem::mkdir(const string& abs_path)
{
    vector<string> v = split(abs_path, '/');
    int size = (int) v.size(); 
    
    FileNode* current = this->root; 
    
    
    for(int i = 1; i < size; ++i)
    {
        if((current->subdirs).count(v[i]))
        {
            current = (current->subdirs)[v[i]];
        }
        else
        {
            FileNode* newNode = new FileNode(v[i], true);
            (current->subdirs)[v[i]] = newNode;
            current = newNode;
        }
    }
    
    return true;
}
bool FileSystem::touch(const string& path)
{
    vector<string> v = split(path, '/');
    int size = (int) v.size(); 
    
    string& filename = v[size - 1];
    
    string dir_path = "";
    
    for(int i = 1; i < size - 1; ++i)
    {
        dir_path = dir_path + "/" + v[i]; 
    }
    
    FileNode* node = this->getDirNode(dir_path);
    
    if(node == NULL)
    {
        cout << "No such directory";
        return false;
    }
    
    (node->subdirs)[filename] = new FileNode(filename, false);
    
    return true;
}
bool FileSystem::ls(const string& path)
{
    FileNode* current = this->getDirNode(path);
    
    if(!current)
    {
        cout << "No such directory\n";
        return false;
    }
    
    unordered_map<string, FileNode*>::iterator it = (current->subdirs).begin(), e = (current->subdirs).end();
    
    while(it != e)
    {
        cout << it->first << "\n";
        ++it;
    }
    
    return true;
}

bool FileSystem::recur_ls(const string& path)
{
    FileNode* current = this->getDirNode(path);
    if(current == NULL)
    {
        cout << "No such directory\n";
        return false;
    }
    vector<string> s;
    this->dfs(current, s);
    return true;
}

void FileSystem::dfs(FileNode* current, vector<string>& paths)
{
    // Base Case: 
    if(!current) return;
    
    paths.push_back(current->name);
    
    for(const string& s : paths)
    {
        cout << s << "/";
    }
    cout << '\n';

    unordered_map<string, FileNode*>::iterator it = (current->subdirs).begin(), e = (current->subdirs).end();
    
    while(it != e)
    {
        dfs(it->second, paths);
        ++it;
    }
    
    paths.pop_back();
    
    return;
}
bool FileSystem::rmdir(const string& path)
{
    vector<string> v = split(path, '/');
    
    int size = (int) v.size();
    
    FileNode* current = this->root;
    FileNode* parent = NULL;
    
    for(int i = 1; i < size; ++i)
    {
        if((current->subdirs).count(v[i]))
        {
            parent  = current;
            current = (current->subdirs)[v[i]];
        }
        else
        {
            cout << "No such directory\n";
            return false;
        }
    }
    
    
    (parent->subdirs).erase(v[size - 1]);
    this->dlt_recur(current);
    return true;
}
bool FileSystem::mv(const string& src, const string& dst)
{
    // mv /darkness/batman  /joker/hello/batman
    vector<string> s1 = split(src, '/');
    int size1 = (int) s1.size();
    vector<string> s2 = split(dst, '/');
    int size2 = (int) s2.size();
    
    FileNode* source = this->root;
    FileNode* parent_source  = NULL;
    
    for(int i = 1; i < size1; ++i)
    {
        if((source->subdirs).count(s1[i]))
        {
            parent_source  = source;
            source = (source->subdirs)[s1[i]];
        }
        else
        {
            cout << "No such source directory\n";
            return false;
        }
    }
    FileNode* parent_dst = this->root;
    for(int i = 1; i < size2; ++i)
    {
        if((parent_dst->subdirs).count(s2[i]))
        {
            parent_dst = (parent_dst->subdirs)[s2[i]];
        }
        else
        {
            cout << "No such destination directory\n";
            return false;
        }
    }
    (parent_source->subdirs).erase(source->name);
    (parent_dst->subdirs)[source->name] = source;
    return true;
}
int main()
{
   
    FileSystem* f = new FileSystem();
    
    f->mkdir("/hello");
    f->mkdir("/darkness");
    f->mkdir("/hello/joker");
    f->mkdir("/darkness/batman");
    f->touch("/darkness/batman/filetext.txt");
    
    f->recur_ls("/");
    
    cout << "------------------" << '\n';
    
    f->mv("/hello/joker", "/darkness/batman");
    
    f->recur_ls("/");
    
    delete f;
    return 0;
}