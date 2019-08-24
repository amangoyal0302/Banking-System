//||_________________________________________________________________________________________________________________
//||
//||                                                                 _____DATA DTRUCTURE PROJECT-SEMESTER 3_______                                                                                                                                                                                                         ||/                                                                                                                                                                                                                                                              //
//||__________________________________________BANKING SYSTEM________________________________________________________

//header file
#include<bits/stdc++.h>
#include<cstring>
using namespace std;

//Creating a class Bank_account
class Bank_account
{
private:
    int acc_no;
    int pin_no;
    char acc_type[100];

public:
    char customer_name[100];
    double balance;
    void get_data(int);
    void del_data();
    void print_data();
    void deposit_balance(double);
    int withdrawn_balance(double,int);
    void update_pin_no();
    bool check_acc(int);
    bool check_pin(int);
    char* getName();
    char* get_acctype();
    int check_duplicate(int*,int,int);
    friend viewAllBooks(Bank_account *cust,int &n);
    friend storeData(Bank_account *cust);
};

// An AVL tree node
struct Node
{
    Bank_account key;
    struct Node *left;
    struct Node *right;
    int height;
};

// A utility function to get maximum of two integers
int max(int a, int b);

// A utility function to get height of the tree
int height(struct Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
    return (a > b)? a : b;
}

/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
struct Node* newNode(Bank_account key)
{
    struct Node* node = (struct Node*)
                        malloc(sizeof(struct Node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;  // new node is initially added at leaf
    return(node);
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct Node *rightRotate(struct Node *y)
{
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct Node *leftRotate(struct Node *x)
{
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    // Return new root
    return y;
}

// Get Balance factor of node N
int getBalance(struct Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Recursive function to insert key in subtree rooted
// with node and returns new root of subtree.
struct Node* insert(struct Node* node, Bank_account key)
{
    /* 1.  Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(key));

    if (key.balance < node->key.balance)
        node->left  = insert(node->left, key);
    else if (key.balance > node->key.balance)
        node->right = insert(node->right, key);
    else // Equal keys are not allowed in BST
        return node;

    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
                           height(node->right));

    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if (balance > 1 && key .balance< node->left->key.balance)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key.balance > node->right->key.balance)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key.balance > node->left->key.balance)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key.balance < node->right->key.balance)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}
struct Node * minValueNode(struct Node* node)
{
    struct Node* current = node;

    /* loop down to find the leftmost leaf */
     while (current->left != NULL)
        current = current->left;

    return current;
}

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
struct Node* deleteNode(struct Node* root, Bank_account key)
{
    // STEP 1: PERFORM STANDARD BST DELETE

    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if ( key.balance < root->key.balance )
        root->left = deleteNode(root->left, key);

    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if( key.balance > root->key .balance)
        root->right = deleteNode(root->right, key);

    // if key is same as root's key, then This is
    // the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct Node *temp = root->left ? root->left :
                                             root->right;

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp; // Copy the contents of
                            // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            struct Node* temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->key.balance = temp->key.balance;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
      return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// A utility function to print preorder traversal
// of the tree.
// The function also prints height of every node
void preOrder(struct Node *root)
{

    if(root != NULL)
    {
        cout<<root->key.customer_name<<" ";
        cout<<"your account have "<<root->key.balance<<  " money";
        cout<<endl;
        preOrder(root->left);
        preOrder(root->right);
    }
}
//function to calculate total amount of money in bank
int preorder2(struct Node *root,int s)
{

    if(root != NULL)
    {
        s+=root->key.balance;
       s= preorder2(root->left,s);
        s=preorder2(root->right,s);

    }
    return s;
}
//Writing the data into the file.
void storeData(Bank_account *cust,int n)
{
    ofstream fout;
    fout.open("bank_account.dat",ios::out | ios::binary);
    int i=0;
    while(i<n){
        fout.write((char *)&cust[i],sizeof(Bank_account));
        i++;
    }
    fout.close();
}
//This function calculates the number of digits of an account number
int chk_accdigit(int acc)
{
int c=1;
if(acc<1000000||acc>9999999)
c=0;
return c;
}
//This function calculates the number of digits of a pin number
int chk_pindigit(int pin)
{
int c=1;
if(pin<1000||pin>9999)
c=0;
return c;
}
//This function returns type of an account(Current/Savings)
char* Bank_account::get_acctype()
{
return acc_type;
}

//This function checks uniqueness of an account number
//we use binary search over there using avl trees in logarthmic time
int Bank_account::check_duplicate(int prev[],int j,int acc_no)
{
for(int i=0;i<j;i++)
{
if(prev[i]==acc_no)
return 1;
}
return 0;
}
//This function is used to input all the information from user
void Bank_account::get_data(int f)
{
static int count=0;
static int prev[99999];
if(f!=0)
{
for(int i=0;i<count;i++)
{
if(prev[i]==f)
prev[i]=0;
}
return;
}
while(1)
{
cout<<"\nEnter account no.(7 digit)-";
cin>>acc_no;
if(chk_accdigit(acc_no)!=1)
{
cout<<"Invalid account no. entered!!(check no. of digits)\n";
continue;
}
prev[count]=acc_no;
if(check_duplicate(prev,count,acc_no)!=0)
{
cout<<"\nThis account no. already exists!!Please enter a different account no.\n";
continue;
}
break;
}
count++;
cout<<"\nEnter customer name:";
cin.ignore();
cin.getline(customer_name,100);
while(1)
{
cout<<"\nEnter account type(Current/Savings):";
cin.getline(acc_type,100);
if(strcmp(acc_type,"Current")==0||
strcmp(acc_type,"Savings")==0)
break;
cout<<"Invalid account type entered!!";
}
while(1)
{
cout<<"\nEnter pin no.(4 digit)-";
cin>>pin_no;
if((chk_pindigit(pin_no))==1)
break;
cout<<"Invalid pin no. entered!!(check no. of digits)\n";
}
if(strcmp(acc_type,"Savings")==0)
{
while(1)
{
cout<<"\nEnter account balance(Rs.):";
cin>>balance;
if(balance>=100)
break;
cout<<"Invalid amount given!!Minimum Rs. 100 must be deposited initially... \n";
}
}
if(strcmp(acc_type,"Current")==0)
{
while(1)
{
cout<<"\nEnter account balance(Rs.):";
cin>>balance;
if(balance>=1000)
break;
cout<<"Invalid amount given!!Minimum Rs. 1000 must be deposited initially... \n";
}
}
cout<<"\nCongratulations!"<<customer_name<<",Your account is successfully created.\n";

}
//This function removes all information of an account which is going to be closed
void Bank_account::del_data()
{
acc_no=0;
pin_no=0;
balance=0.0;
}
//This function is used to deposit a valid amount of balance in an account
void Bank_account::deposit_balance(double blnc)
{
cout<<"Please wait.";
//delay loop
for(int q=0;q<15;q++){
for(int i=0;i<100000;i++){

}
cout<<".";
}
cout<<endl;
if(blnc<=0)
{
cout<<"\nTransaction unsucessful!!Please enter a valid amount.\n";
return;
}
balance+=blnc;
cout<<"Transaction completed succcessfully.\n";
}
//This function is used to withdraw a valid amount of balance from an account
 int Bank_account::withdrawn_balance(double blnc,int f)
{
char ch;
cout<<"Please wait.";
for(int q=0;q<15;q++){
for(int i=0;i<100000;i++){

}
cout<<".";
}
cout<<endl;
if(blnc<=0)
{
cout<<"\nTransaction unsuccessful!! Please enter a valid amount.\n";
return 0;
}
if(blnc>balance)
{
cout<<"\nTransaction unsucessful!! Your current balance is:"<<balance<<"\n"<<"you are unable to request for:"<<blnc<<"\n\n";
return 0;
}
if(f==1)
{
if((double)(balance-blnc)<100)
{
cout<<"\nTransaction unsucessful!!Minimum Rs.100 must be stored to maintain a savings account."<<"\n\n";
cout<<"Do you want to withdraw all money and close your account?(Y/N):";
cin>>ch;
if(ch=='Y'||ch=='y')
return 1;
return 0;
}
}
else if(f==0)
{
if((double)(balance-blnc)<1000)
{
cout<<"\nTransaction unsucessful!!Minimum Rs.1000 must be stored to maintain a current account."<<"\n\n";
 cout<<"Do you want to withdraw all money and close your account?(Y/N):";
cin>>ch;
if(ch=='Y'||ch=='y')
return 1;
return 0;
}
}
balance-=blnc;
cout<<"Transaction completed succcessfully.\n";
}

//This function is used to check whether an account number exists or not
 bool Bank_account::check_acc(int acc)
{
if(acc_no==acc)
return true;
else
return false;
}

//This function is used to check whether a pin number exists or not
bool Bank_account::check_pin(int pin)
{
if(pin_no==pin)
return true;
else
return false;
}
//This function is used to update a customer's pin number
void Bank_account::update_pin_no()
{
int pin1,pin2;
while(1)
{
cout<<"Enter old pin no.";
cin>>pin1;
if(pin1==pin_no)
break;
cout<<"Invalid pin no. entered!!\n";
return;
}
while(1)
{
cout<<"\nEnter new pin no.(4 digit)-";
cin>>pin2;
if((chk_pindigit(pin2))==1)
break;
cout<<"Pin no. can't be changed,invalid no. of digits entered!!\n";
}
pin_no=pin2;
cout<<"Pin no. successfully updated.\n";
}

//This function is used to print all the information of a customer
void Bank_account::print_data()
{
cout<<"\n\nAccount no.-:"<<acc_no;
cout<<"\n\nAccount type:"<<acc_type;
cout<<"\n\nAccount balance:Rs."<<balance<<"\n";
}

//This function returns name of a customer
 char* Bank_account::getName()
{
return customer_name;
}

//Reading the complete data from the file.
void viewAllAccounts(Bank_account *cust,int &n)
{
    ifstream fin;
    fin.open("bank_account.dat",ios::in | ios::binary);
    if(!fin)
    {
        cout<<"File does not exists\n";
        return;
    }
    fin.read((char *)&cust[n],sizeof(Bank_account));
    while(!fin.eof())
    {
        n++;
        fin.read((char *)&cust[n],sizeof(Bank_account));
    }
    fin.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                           ________ MAIN FUNCTION___________                                                                                                                                                                                                          //
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
int main()
{
//required variables declaration
int num1=0,i,acc,pin,ch,ac_flag,ch1,ch2,ch3,del,endd=0,savings=0,cr=0,admin=321998;
bool flag;
double amount;
char c,op;
Bank_account *cust=new Bank_account[99999];
cout<<"\n\n";

//base of tree structure
Node*root_s=NULL;
Node *root_c=NULL;

viewAllAccounts(cust,num1);

for(int zz=0;zz<num1;zz++)
{
   if(strcmp((cust[zz].get_acctype()),"Savings"))
        root_c= insert(root_c,cust[zz]);
    else if(strcmp((cust[zz].get_acctype()),"Current"))
        root_s=insert(root_s,cust[zz]);
}

Node *curr_s=root_s;//temporary pointer of current type
Node *curr_c=root_c;//temporary pointer of saving type

while(1)
{
//Main menu
cout<<"\nWELCOME TO BANK\n---------------------\n""\n1.ADMINISTRATOR(create account(s)/get max current account/get max saving acc/total savings of bank/total curr acc deposit).\n2.CUSTOMER(View/Modify existing account(s)).\n3.EXIT.\n\nEnter your choice:";
cin>>ch1;
switch(ch1)//switch case 1
{
case 1:
cout<<"Enter administrator password:";
cin>>admin;

//........................admin login password................................

if(admin!=321998)
{
cout<<"\nInvalid Password!!!";
break;
}
while(1)
{
    cout<<"\nAdmin menu menu\n---------------------\n""\n1.Add Acccount.\n2.Total Savings Money In Bank.\n3.Customer Having Max Saving.\n4.Customer Having Max Current Balance\n5.Exit operation for this Admin.\n\nEnter your choice:";
cin>>ch3;
switch(ch3)
{
case 1:
     cust[num1].get_data(0);
    if(strcmp((cust[num1].get_acctype()),"Savings"))
    root_c=insert(root_c,cust[num1]);
    else
    root_s=insert(root_s,cust[num1]);
    num1++;
    break;

case 2:
    savings=preorder2(root_s,0);
    cout<<"Toatal Money In Saving account In bank : "<<savings<<endl;
    break;

case 3:
    curr_s=root_s;
    while(curr_s->right!=NULL)
        curr_s=curr_s->right;
    cout<<"account type :"<<curr_s->key.get_acctype()<<endl;
    cout<<"customer name:"<<curr_s->key.customer_name<<endl;
    cout<<"Customer balance:"<<curr_s->key.balance<<endl;
    break;

case 4:
    curr_c=root_c;
    while(curr_c->right!=NULL)
        curr_c=curr_c->right;
    cout<<"account type :"<<curr_c->key.get_acctype()<<endl;
    cout<<"customer name:"<<curr_c->key.customer_name<<endl;
    cout<<"Customer balance:"<<curr_c->key.balance<<endl;
    break;

case 5:
    endd=1;
    break;
default:
    cout<<"Invalid Choice"<<endl;
}
if(endd==1)
    break;
}
//cr++;
break;

/****************************************************************************************************************************************************************************/
case 2:
//if(cr<=0)
if(num1<=0)
{
cout<<"Operation failed!!There is no account present in the bank.\n";
break;
}
while(1)//loop1
{
do
{
cout<<"\nEnter account no.:";
cin>>acc;
for(i=0;i<num1;i++)
{
if(flag=cust[i].check_acc(acc))
break;
}
if(i==num1)
cout<<"\nAccount no. does not exist!!\n";
}
while(flag==false);
cout<<"\nHello "<<cust[i].getName()<<",welcome to your account...\n";
cout<<"\nEnter Pin number(Be careful):";
cin>>pin;
if((flag=cust[i].check_pin(pin))==false)
{
cout<<"Invalid pin no. entered!!\nOperation aborted for this customer!!\n";
op='N';
break;
}
while(1)//while2
{
op='Y';
del=0;
//Menu for operations to be performed on an account
cout<<"\nCustomer menu\n---------------------\n""\n1.Account Details.\n2.Depositing Balance.\n3.Withdraw Balance.\n4.Update Pin no.\n5.Exit operation for this customer.\n\nEnter your choice:";
cin>>ch;
switch(ch)
{
case 1:
    cust[i].print_data();
    break;

case 2:
    cout<<"Enter amount to deposit:";
    cin>>amount;
    cust[i].deposit_balance(amount);
    break;

case 3:
    cout<<"Enter amount to withdraw";
    cin>>amount;
    if(strcmp((cust[i].get_acctype()),"Savings"))
    ac_flag=1;
else
    ac_flag=0;
    del=cust[i].withdrawn_balance(amount,ac_flag);
if(del==1)
{
    cout<<"\nAre you sure?(Y/N):";
    cin>>c;
if(c!='Y'&&c!='y')
    break;
    cust[i].del_data();
    cust[i].get_data(acc);
    cr--;
    cout<<"\nAccount Closed.\n";
}
break;

case 4:
    cust[i].update_pin_no();
    break;

case 5:
    op='N';
    break;

default:
    cout<<"Invalid choice!!!!\n";
//end of switch case
}
if(del==1)
    break;
if(op=='N')
    break;
}//end of while2
if(del==1)
    break;
op='N';
cout<<"\nDo you want to continue operation for another customer(Y/N):";
cin>>op;
if(op!='Y'&&op!='y')
    break;
}//end of loop1
break;

case 3:
    storeData(cust,num1);
    cout<<"\nThank You,visit again..\n";return 0;

default:
cout<<"Invalid choice!!!!\n";
}
//end of switch case1

}
//end of main while loop

delete[]cust;//Deallocate memory
}

//________________________________________________   END OF PROJECT_____________________________________________________________________
//____________________________________________________THANK YOU________________________________________________________________________

