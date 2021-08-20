#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include<time.h>

typedef struct user
{
    char email[100];
    char pass[100];
    int isAdmin;
} user;

user temp_user;
int user_counter=0;
int max_user=100;
user users_arr[100];///users array

void add_user(char email[],char pass[],int admin)
{
    strcpy(temp_user.email,email);
    strcpy(temp_user.pass,pass);
    temp_user.isAdmin=admin;
    users_arr[user_counter++]=temp_user;
}
user current_user;

int search_user(char email[],char pass[])
{

    int f=-1;
    int idx;

    for(int i=0; i<user_counter; i++)
    {
        if(strcmp( users_arr[i].email,email)==0 && strcmp( users_arr[i].pass,pass)==0)
        {
            f=1;
            idx=i;

        }
    }
    return f==1?idx:-1;

}


int login(char email[],char pass[])
{
    int idx=search_user(email,pass);
    if(idx!=-1)
    {
        current_user=users_arr[idx];
        if(current_user.isAdmin){
            printf("\n**Admin**\n");
            return 1;
        }
        else{
            printf("\nCustomer: %s\n", current_user.email);
            return 1;
        }
    }
    return 0;
}


int login_display()
{
    printf("enter email and password to login\n");
    printf("email :\n");
    char email[100];
    scanf("%s",email);
    printf("password : \n");
    char pass[100];
    scanf("%s",pass);

    return login(email,pass);

}


int logout()
{

    strcpy(current_user.email,"");
    strcpy(current_user.pass,"");
    printf("\t **you are logout** \n");

}



int register_(char email[],char pass[],int admin)
{

    int idx=search_user(email,pass);
    if(idx==-1)
    {

        add_user(email,pass,admin);
        return 1;
    }
    return 0;
}



typedef struct product
{
    int id,price,quantity;
    char name[100];
} product;


int product_counter=0;
int max_product=500;
product products_arr[500];

int search_product_byname(char name[])
{
    int f=-1,idx;
    for(int i=0; i<product_counter; i++)
    {
        if(strcmp(products_arr[i].name,name)==0)
        {
            f=1;
            idx=i;
        }
    }
    return f==1?idx:-1;
}

void search_product_name(char sname[])
{

    printf("\n\n\t\t>>> >>> Search Result <<< <<<\n\n");



    for(int i=0; i<product_counter; i++)
    {
        if(strcmp(products_arr[i].name,sname)==0)
            printf("Id:(%d), name: %s,  Price: %d,  Quantity: %d\n\n", i+1,products_arr[i].name, products_arr[i].price, products_arr[i].quantity);
    }

}


int search_product_byid(int id)
{
    int f=-1,idx;
    for(int i=0; i<product_counter; i++)
    {
        if(products_arr[i].id==id)
        {
            f=1;
            idx=i;
        }
    }
    return f==1?idx:-1;
}


void show_list_of_product(int with_id)
{
    printf("\t\t****product list ****\n");
    for(int i=0; i<product_counter; i++)
    {
        if(with_id!=1)
            printf("id:(%d), name : %s \n",i+1,products_arr[i].name);
        else
            printf("id: (%d), name : %s \n",products_arr[i].id,products_arr[i].name);
            printf("\tprice : %d\n",products_arr[i].price);
            printf("\tquantity : %d\n\n",products_arr[i].quantity);
    }
}


int add_product(product p)
{
    if(!current_user.isAdmin)
        return 0;
    int idx=search_product_byname(p.name);
    if(idx== -1)
    {
        products_arr[product_counter++]=p;//add new product
    }
    else
    {
        products_arr[idx].quantity+=p.quantity;//update quantity
    }

}
void create_product()
{
    product t;
    printf("product name :");
    scanf("%s",t.name);
    printf("price :");
    scanf("%d",&t.price);
    printf("quantity :");
    scanf("%d",&t.quantity);

    srand(time(0));
    t.id=rand()%10000+rand()%20+2;     // random
    add_product(t);

}

int update_quantiy(int id,int q){
    if(!current_user.isAdmin)
        return 0;
    int idx=search_product_byid(id);
    if(idx==-1)
    {
        printf("\n\t ***cant find the product please try again ***\n");
    }
    else
    {
        products_arr[idx].quantity=q;
        //update quantity
    }
}

int delete_product(int id){         /// delete

    char nullStr[20] = {"\0"};
    if(!current_user.isAdmin)
        return 0;
    int idx = search_product_byid(id);
    if(idx==-1)
    {
        printf("\n\t ***cant find the product please try again ***\n");
    }
    else
    {
        strcpy(products_arr[idx].name, nullStr);                /// delete product name
        products_arr[idx].id = 0;
        products_arr[idx].price = 0;
        products_arr[idx].quantity = 0;
        product_counter--;
        printf("\n\t ***Product removed! ***\n");

    }
}


/********cart*********/
typedef struct cart
{
    product p;
    int idx;
} cart;
int max_cart_item=100;
int cart_counter=0;
cart mycart[100];

int search_mycart(char name[])
{
    int f=-1,idx;
    for(int i=0; i<cart_counter; i++)
    {
        if(strcmp(mycart[i].p.name, name)==0)
        {
            f=1;
            idx=i;
        }
    }
    return f==1?idx:-1;
}
int add_to_cart(product p,int q,int list_num)
{
    /** get a product , quantity ,that product index number **/

    int idx = search_mycart(p.name);
    if(idx == -1)
    {
        cart tcart= {p,list_num};
        mycart[cart_counter]=tcart;    //add new product
        mycart[cart_counter].p.quantity=q;      //
        products_arr[list_num].quantity-q;  // reduce product from total shop quantity
        cart_counter++;
    }
    else
    {
        if(products_arr[list_num].quantity-(mycart[idx].p.quantity+q)<! 0)          // if the product previously added just add quantity to cart and reduce from shops total quantity.
            mycart[idx].p.quantity+=q;//update quantity
        else
        {

            mycart[idx].p.quantity += products_arr[list_num].quantity;
            printf("\n**can't add more item then %d **\n", products_arr[list_num].quantity);
        }
    }
}
void myreceipt()
{
    printf("\t\t*****my receipt ****\n");
    int total_amount =0;
    if(cart_counter==0)
        printf("\nopps! no product is here \n");
    for(int i=0; i<cart_counter; i++)
    {
        printf("%d name : %s \n",i+1,mycart[i].p.name);
        printf("\tprice : %d\n",mycart[i].p.price);
        printf("\tquantity : %d\n",mycart[i].p.quantity);
        total_amount+=(mycart[i].p.price*mycart[i].p.quantity);

    }
    printf("total amount, %d\n",total_amount);
}

void checkout()
{
    if(cart_counter==0)
        printf("\nopps! no product is here \n");
    for(int i=0; i<cart_counter; i++)
    {
        products_arr[mycart[i].idx].quantity -= mycart[i].p.quantity;
    }
    printf("parches successful");
}

void admin_view(){
    printf("\n1.show all product\n2.Create NEW Product\n3.Update product\n4.Delete product\n5.logout\n");
    int x;
     scanf("%d",&x);
     switch (x){
 case 1:
    show_list_of_product(0);
    break;
 case 2:
    create_product();
    break;
case 3:                                             /// Update product
    show_list_of_product(1);
    int y;
    printf("enter product id :");
    scanf("%d",&y);
     int q;
     printf("enter quantity :");
     scanf("%d",&q);
    update_quantiy(y,q);
    break;
case 4:                                             /// Delete product
    show_list_of_product(1);
    int dd;
    printf("enter product id :");
        scanf("%d",&dd);
    delete_product(dd);
    break;

case 5:
    logout();
    break;

     }
}


void coustomer_view(){
    int x;
    char ng[100];

    printf("\n1.Show all Product\n2.my receipt \n3. search product by name\n4. purchase\n5.logout\n");
    scanf("%d",&x);
    switch (x){
    case 1:
        show_list_of_product(0);

        printf("Select an product by entering list ID or enter -1 to to to menu  :");
        int list_idx;
        scanf("%d",&list_idx);
        if(list_idx==-1){
            break;
        }else{
            if(list_idx>product_counter)
                printf("\t**Select correct list id \n");

            printf("\nEnter quantity : ");
            int q;
            scanf("%d",&q);

            add_to_cart(products_arr[list_idx-1],q,list_idx-1);

            printf("\n\t *This item added to your cart* \n");

        }

        break;

    case 2:
        myreceipt();
        break;
    case 3:

        printf("Enter Product name:  ");
        getchar();
        gets(ng);
        search_product_name(ng);            // search product

        break;
    case 4:             //Purchase
        checkout();
        cart_counter=0;

        break;
    case 5:
        logout();
        break;
    default:
        break;
    }

}
void Goto()
{
    while(1)
    {
        if(strcmp(current_user.email,"")!=0)
            if(current_user.isAdmin)
                admin_view();
            else
                coustomer_view();
        else
            break;

    }
}
int main()
{

    add_user("ad","pass",1);
    add_user("ccc","pass",0);


    int x;
    do
    {

        if(login_display())
        {

            printf("\nWelcome %s\n",current_user.email);
            Goto();
        }
        else
        {
            printf("incorrect email or pass , try again ");
        }

    }while(1);

    return 0;
}
