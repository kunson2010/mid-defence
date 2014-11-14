#include <stdio.h>
#include <stdlib.h>

/* 
 *
 * 0. 构成一个12节点的循环链表, 
 *      每个节点的权重为0
 *      标注大调音阶的位置
 *      标注小调音阶的位置
 *
 * 1. 对于给定的和弦1, 分析其
 *                              大小调, 
 *                              音级, 
 *                              和弦种类: 三和弦("V/C"), 七和弦, 十一和弦, 十三和弦, 六和弦...
 *
 * 2. 对于大调,按照 两全一半三全一半的关系, 从链表的第0节点标出大调的7个音级, 每个音赋予权重1
 *    对于小调,按照 全半全全半的关系, 从链表的第0节点标出小调的7个音级
 *
 * 3. (目前只考虑三和弦)确定音级的位置, 按照这个和弦的类型, 定位根音权重赋值4, 定位3音权重赋值2, 定位五音权重赋值3 
 *
 * 4. 对于给定的和弦2, 在同一个数据结构上做同样的操作?
 *
 *
 */

/* 
 * 
 * 在自然大调与自然小调的各个音级上，都可以构成一个它所属调式的自然音阶三和弦。
 *
 *  **  目前局限是只考虑了自然调式的和弦, 没考虑和声调式和旋律调式.
 *
 * 自然大调:
 *      I, IV, V        大三和弦 Maj_triad
 *      ii, iii, vi     小三和弦 Min_triad
 *      vii*            减三和弦 Dim_triad
 *
 * 自然小调
 *      III, VI, VII    大三和弦
 *      i, iv, v        小三和弦
 *      ii*             减三和弦
 *
 *
 * 大三和弦  1音 (4半音) 3音 (3半音) 5音   
 * 小三和弦  1音 (3半音) 3音 (4半音) 5音     
 * 增三和弦  1音 (4半音) 3音 (4半音) 5音     
 * 减三和弦  1音 (3半音) 3音 (3半音) 5音     
 *
 *
 */

#define PRT printf("---->line %d\n", __LINE__);
#define P_line printf("\n");


//全局变量
typedef struct {           //三和弦的类型,  "Maj_1" 对应 "大三和弦"
    char chord_level[32];
    char chord_type[32];
} c_t;
c_t chord_type[14]={
    {"Maj_1","Maj_tri"},   //大调音阶的第一级音的三和弦 是 大三和弦 Do Mi So, "I"
    {"Maj_2","Min_tri"},   
    {"Maj_3","Min_tri"},  
    {"Maj_4","Maj_tri"}, 
    {"Maj_5","Maj_tri"},
    {"Maj_6","Min_tri"},   
    {"Maj_7","Dim_tri"},   
    {"Min_1","Min_tri"},   
    {"Min_2","Dim_tri"},   
    {"Min_3","Aug_tri"},   
    {"Min_4","Min_tri"},   
    {"Min_5","Maj_tri"},   
    {"Min_6","Maj_tri"},   
    {"Min_7","Dim_tri"},   
};

typedef struct {                //三和弦的I,III,V音级之间的半音间隔
    char chord_type[32];
    char chord_step[32];
} c_s;
c_s chord_step[4]={
    {"Maj_tri","43"},
    {"Min_tri","34"},
    {"Dim_tri","33"},
    {"Aug_tri","44"},
};

typedef struct {
    int level;
    char is1;
    char is2;
} c_c;
c_c chord_circle[7]={   // 第一个版本的五度圈, 和弦1的level所在行is1字段标注‘*'， 和弦2的is2字段标注’*'
    {1,' ', ' '},
    {5,' ', ' '},
    {2,' ', ' '},
    {6,' ', ' '},
    {3,' ', ' '},
    {7,' ', ' '},
    {4,' ', ' '}
};

typedef struct node{
    int Maj_pos;       // 在大调的音级位置, 如: I, ii,  iii,  IV, V, vi, vii*,   取值1~7, 0表示不在大调音阶中
    int Min_pos;       // 在小调的音级位置, 如: I, ii,  iii,  IV, V, vi, vii*,   取值1~7, 0表示不在大调音阶中
    int chord1_weight; // 在和弦1中的权重
    int chord2_weight; // 在和弦2中的权重
    struct node * next;
} bs_node;

bs_node *head, *p, *q;


//函数声明
bs_node * creat(void);
bs_node * move(bs_node *, int );
void analyze(char * , char *, int *, int *, char *);
void set_scale(char *mode, bs_node *init, const int chord_num, const int level, char * step_interval);
/*
int chord2pos(char *);
int d_bs(char *, char *);
*/




//主函数-------------
main(int argc, char **argv)
{
    int  i, j, j1, j2;
    char chord1[32]="", chord2[32]="";
    char mode[32]="";
    int  tonic_pos=-1;
    int  level=-1;
    char step_interval[32]="";
    int  delta=0;

        
    head=creat();

    /*
    for(p=head, i=0; i<12; i++, p=move(p,1)) {
        printf("Maj_pos=%d, Min_pos=%d, chr1_weight=%d, char2_weight=%d\n", 
                p->Maj_pos, p->Min_pos, p->chord1_weight, p->chord2_weight);    
    }
    */

    printf("\n\n\n\n"); 

        

    for( ; ; ) {  
        clean();
        delta=0;

        printf("\n\ninput chord1:\t");
        scanf("%s", chord1);
        printf("\n\ninput chord2:\t");
        scanf("%s", chord2);
        P_line
        

        /****/ //strcpy(chord1, argv[1]);
        /****/ //strcpy(chord2, argv[2]);


        // 1. 解析第一个和弦, 得到调性, 在BS的主音位置, 根音所在的级数, 和弦的音程关系
        analyze(chord1, mode, &tonic_pos, &level, step_interval);
        printf("chord1-->mode=%s, tonis pos on BS=%d, root level=%d, step_interval=%s\n", 
                mode, tonic_pos, level, step_interval);

        // 1.1 标注chord_circle
        for(i=0; i<7; i++){
            if( level==chord_circle[i].level ) {
                //chord_circle[i].is1='*';
                j1=i;
            } 
        } 

        // 2.  标注和弦1
        // 2.1 定位tonic在BS中的位置
        for (i=0, p=head; i<tonic_pos; i++, p=move(p,1) ) {
            ;
        } 

        // 3. 按调性 在BS中设置自然音阶, 权=1; 按根音所在的级数, 和弦的音程关系, 设置和弦的III, V音级的权
        set_scale(mode, p, 1, level, step_interval);

        // -----------------和弦2---------------
        
        // 1. 解析第二个和弦, 得到调性, 在BS的主音位置, 根音所在的级数, 和弦的音程关系
        analyze(chord2, mode, &tonic_pos, &level, step_interval);
        printf("chord2-->mode=%s, tonis pos on BS=%d, root level=%d, step_interval=%s\n", 
                mode, tonic_pos, level, step_interval);

        // 1.1 标注chord_circle
        for(i=0; i<7; i++){
            if( level==chord_circle[i].level ) {
                //chord_circle[i].is2='*';
                j2=i;
            } 
        } 

        // 2.  标注和弦2
        // 2.1 定位tonic在BS中的位置
        for (i=0, p=head; i<tonic_pos; i++, p=move(p,1) ) {
            ;
        } 

        // 3. 按调性 在BS中设置自然音阶, 权=1; 按根音所在的级数, 和弦的音程关系, 设置和弦的III, V音级的权
        set_scale(mode, p, 2, level, step_interval);


        ///*
        for(p=head, i=0; i<12; i++, p=move(p,1)) {
            printf("BS[%.2d] Maj_pos=%d, Min_pos=%d, chr1_weight=%d, char2_weight=%d\n", 
                 i, p->Maj_pos, p->Min_pos, p->chord1_weight, p->chord2_weight);    
        }
        //*/

        P_line
        P_line

        for( i=0, p=head; i<12; i++, p=move(p,1) ) {
            if((p->chord2_weight)>=(p->chord1_weight)) {
                delta += (p->chord2_weight)-(p->chord1_weight);
            }
        } 

        j = abs(j2-j1);
        j<4? j:7-j;

        printf("        Distance of %s --->%s = %d\n", chord1, chord2, delta+(j<4? j:7-j));

    }   // for( ; ; ) {  }
}

      
// 清空BS
void clean()
{
    bs_node *p;
    int i;

    p=head;
    for(i=0; i<12; i++, p=move(p, 1)){
        p->Maj_pos=0; 
        p->Min_pos=0; 
        p->chord1_weight=0; 
        p->chord2_weight=0;    
    }
}

// 3. 按调性 在BS中设置自然音阶, 权=1; 按根音所在的级数, 和弦的音程关系, 设置和弦的III, V音级的权
void set_scale(char * mode, bs_node * init, const int chord_num, const int level, char * step_interval)
{
    bs_node *p, *q;
    int i;


    // 0. 清空原数据
    for(i=0, p=init; i<12; i++, p=move(p,10) ){
        p->Maj_pos=p->Min_pos=0;
    } 

    p=init;
    // 1. 对chord1 按调性设置自然音阶, 权=1
    if( 1==chord_num){ 
        if ( 0==strcmp(mode, "Maj") ) { 
            // 标注大调音阶的位置
            p->Maj_pos=1; p->chord1_weight=1; 
            p=move(p, 2); p->Maj_pos=2; p->chord1_weight=1; //全音
            p=move(p, 2); p->Maj_pos=3; p->chord1_weight=1; //全音
            p=move(p, 1); p->Maj_pos=4; p->chord1_weight=1; //半音
            p=move(p, 2); p->Maj_pos=5; p->chord1_weight=1; //全音
            p=move(p, 2); p->Maj_pos=6; p->chord1_weight=1; //全音
            p=move(p, 2); p->Maj_pos=7; p->chord1_weight=1; //全音


            // 2. 按根音所在的级数, 和弦的音程关系, 设置和弦的III, V音级的权
           
            // 2.1  设置I音级的权重=4
            for(p=init; ; p=move(p, 1) ) {
                if( level == p->Maj_pos){
                    break;
                } 
            }
            p->chord1_weight=4;

            // 2.2. 设置III音级的权重=2
            p=move(p, (step_interval[0]-0x30) ); 
            p->chord1_weight=2;

            // 2.2. 设置V音级的权重=3
            p=move(p, (step_interval[1]-0x30) );
            p->chord1_weight=3;
        }


        if ( 0==strcmp(mode, "Min") ) { 
            // 标注小调音阶的位置
            p->Min_pos=1; p->chord1_weight=1; 
            p=move(p, 2); p->Min_pos=2; p->chord1_weight=1; //全音
            p=move(p, 1); p->Min_pos=3; p->chord1_weight=1; //半音
            p=move(p, 2); p->Min_pos=4; p->chord1_weight=1; //全音
            p=move(p, 2); p->Min_pos=5; p->chord1_weight=1; //全音
            p=move(p, 1); p->Min_pos=6; p->chord1_weight=1; //全音
            p=move(p, 2); p->Min_pos=7; p->chord1_weight=1; //全音

            // 2. 按根音所在的级数, 和弦的音程关系, 设置和弦的III, V音级的权
           
            // 2.1  设置I音级的权重=4
            for(p=init; ; p=move(p, 1) ) {
                if( level == p->Min_pos){
                    break;
                } 
            }
            p->chord1_weight=4;

            // 2.2. 设置III音级的权重=2
            p=move(p, (step_interval[0]-0x30) ); 
            p->chord1_weight=2;

            // 2.2. 设置V音级的权重=3
            p=move(p, (step_interval[1]-0x30) );
            p->chord1_weight=3;
        }
    } // if( 1==chord_num){ 

    if( 2==chord_num){ 
        if ( 0==strcmp(mode, "Maj") ) { 
            // 标注大调音阶的位置
            p->Maj_pos=1; p->chord2_weight=1; 
            p=move(p, 2); p->Maj_pos=2; p->chord2_weight=1; //全音
            p=move(p, 2); p->Maj_pos=3; p->chord2_weight=1; //全音
            p=move(p, 1); p->Maj_pos=4; p->chord2_weight=1; //半音
            p=move(p, 2); p->Maj_pos=5; p->chord2_weight=1; //全音
            p=move(p, 2); p->Maj_pos=6; p->chord2_weight=1; //全音
            p=move(p, 2); p->Maj_pos=7; p->chord2_weight=1; //全音

            // 2.1  设置I音级的权重=4
            for(p=init; ; p=move(p, 1) ) {
                if( level == p->Maj_pos){
                    break;
                } 
            }
            p->chord2_weight=4;

            // 2.2. 设置III音级的权重=2
            p=move(p, (step_interval[0]-0x30) ); 
            p->chord2_weight=2;

            // 2.2. 设置V音级的权重=3
            p=move(p, (step_interval[1]-0x30) );
            p->chord2_weight=3;
        }

        if ( 0==strcmp(mode, "Min") ) { 
            // 标注小调音阶的位置
            p->Min_pos=1; p->chord2_weight=1; 
            p=move(p, 2); p->Min_pos=2; p->chord2_weight=1; //全音
            p=move(p, 1); p->Min_pos=3; p->chord2_weight=1; //半音
            p=move(p, 2); p->Min_pos=4; p->chord2_weight=1; //全音
            p=move(p, 2); p->Min_pos=5; p->chord2_weight=1; //全音
            p=move(p, 1); p->Min_pos=6; p->chord2_weight=1; //全音
            p=move(p, 2); p->Min_pos=7; p->chord2_weight=1; //全音

            // 2.1  设置I音级的权重=4
            for(p=init; ; p=move(p, 1) ) {
                if( level == p->Min_pos){
                    break;
                } 
            }
            p->chord2_weight=4;

            // 2.2. 设置III音级的权重=2
            p=move(p, (step_interval[0]-0x30) ); 
            p->chord2_weight=2;

            // 2.2. 设置V音级的权重=3
            p=move(p, (step_interval[1]-0x30) );
            p->chord2_weight=3;
        }
    } // if( 2==chord_num){ 
        
}



// 生成bs链表，返回第一个节点的指针
bs_node * creat()
{
    int i;
    bs_node *head, *p, *q;

    p=(bs_node(*))malloc(sizeof(bs_node));
    memset(p, 0x0, sizeof(bs_node));
    head=p;

    for( i=0; i<11; i++) {  
        p->next=malloc(sizeof(bs_node));
        q=p->next;
        memset(q, 0x0, sizeof(bs_node));
        q->next=NULL;
        p=q;
    } 
    q->next=head;

    return (head);
}


// 在bs链表上 移动 n个节点
bs_node * move(bs_node * p, int n)
{
    int i;

    for (i=0; i<n ; i++) {
        p=p->next;
    }
    return (p);
}



/* 
 * 1. 对于给定的和弦1, 分析其
 *                              调性: 大小调与调号 (tonal-mode(tonality)  & key)
 *                              音级, (I, II, III, IV, V, VI, VII......) 
 *                              和弦种类: 三和弦("V/C"), 七和弦, 十一和弦, 十三和弦, 六和弦...
 * 目前的局限性在于 
 *              1. 输入数据是字符串"V/C", 最终应为MusicXML
 *              2. 目前只处理了三和弦, 最终应处理 七, 十一, 十三和弦等等
 *
 *
 *
 *  input: 
 *      str: "V/C"
 *
 *  output:
 *      mode: "Maj" or "Min"
 *      tonic_pos: 0 or 1 or 2 or ... or 11
 *      chord_level: 1 or 2 or ... 7 
 *      step_interval: "43" or "34" or "33" or "44"
 *
 */
void analyze(char * str, char * out_mode, int * out_tonic_pos, int * out_chord_level, char * out_step_interval)
{
    char maj_key[15][32]={"C", "G", "D", "A", "E", "B", "F_sharp", "C_sharp", 
        "F", "B_flat", "E_flat", "A_flat", "D_flat", "G_flat", "C_flat"};
    char min_key[15][32]={"a", "e", "b", "f_sharp", "c_sharp", "b_sharp", 
        "d_sharp", "a_sharp", "d", "g", "c", "f", "b_flat", "e_flat", "a_flat"};

    typedef struct {     // 各个调的主音在bs中的位置, 与调性(大小调, 调号)相关
        char key[32];
        char pos;
    } k_p;
    k_p key_pos[29]={
        {"C", 0},{"c", 0},
        {"C_sharp", 1},{"c_sharp", 1}, {"D_flat", 1},{"d_flat", 1},
        {"D", 2},{"d", 2}, 
        {"E_flat", 3},{"e_flat", 3},
        {"E", 4},{"e", 4},
        {"F", 5},{"f", 5},
        {"F_sharp", 6},{"f_sharp", 6}, {"G_flat", 6}, {"g_flat", 6},
        {"G", 7},{"g", 7},
        {"A_flat", 8},{"a_flat", 8},
        {"A", 9},{"a", 9},
        {"B_flat", 10},{"b_flat", 10},
        {"B", 11},{"b", 11},{"C_flat", 11}, 
    };

    typedef struct {            // "V"-->"5"
        char chord[32];
        char level[32];
    } l_n;
    l_n level_num[21]={
        {"I", "1"}, {"i", "1"}, {"i*", "1"},
        {"II", "2"}, {"ii", "2"}, {"ii*", "2"},
        {"III", "3"}, {"iii", "3"}, {"iii*", "3"},
        {"IV", "4"}, {"iv", "4"}, {"iv*", "4"},
        {"V", "5"}, {"v", "5"}, {"v*", "5"},
        {"VI", "6"}, {"vi", "6"}, {"vi*", "6"},
        {"VII", "7"}, {"vii", "7"}, {"vii*", "7"},
    };

    char mode[32]="";
    char key[32]=""; 
    char chord[32]=""; 
    char  level[32]="";
    char chord_level[32]="";  //"Maj_1"
    char step_inteval[32]=""; //和弦中几个音之间的音程步幅关系. **不足:只考虑了三和弦.
    int  i=0;
    int  j=0;
    int  pos_on_bs; // position on BS, 主音在BS上的位置
    char *p;


    for(i=0, p=str; *p!='/'; i++, p++) { // 获得 chord
        chord[i]=*p;
    }
    for(i=0, p++; *p!=NULL; i++, p++) { // 获得  mode & key
        key[i]=*p;
    }
    //printf("chord=%s, key=%s\n", chord, key);

    // 1. 判断调性 mode
    for (i=0; i<15; i++ ) {
        if (strcmp(key, maj_key[i])==0) 
            strcpy(mode, "Maj");
        if (strcmp(key, min_key[i])==0) 
            strcpy(mode, "Min");
    } 
    strcpy(out_mode, mode);
    //printf("The tonal mode is %s\n", mode);

    // 2. 判断主音在bs的位置, 
    for (i=0; i<29; i++) {
        if (strcmp(key, key_pos[i].key)==0) {
            pos_on_bs=key_pos[i].pos;
        }
    }
    *out_tonic_pos = pos_on_bs;
    //printf("The tonic pos on BS is %d\n", pos_on_bs);

    // 3. 得到root音的级数  "V"-->5
    for ( i=0; i<21; i++ ) {
        if( strcmp(chord, level_num[i].chord)==0 ){
            strcpy(level, level_num[i].level);
        }  
    }
    *out_chord_level = atoi(level);
    //printf("%s in level of %d\n", chord, *out_chord_level);


    // 4. 和弦种类 mode+chord名字得到的级数序号, 
    sprintf(chord_level, "%s_%s", mode, level);
    //printf("%s\n", chord_level);


    // 5. 按和弦种类查询到3音级和5音级的半音个数, 
    for(i=0; i<14; i++){
        if( strcmp(chord_level, chord_type[i].chord_level)==0 ){ 
            for(j=0; j<4; j++ ){
                if(0==strcmp(chord_type[i].chord_type, chord_step[j].chord_type)){
                    strcpy(step_inteval, chord_step[j].chord_step);
                } 
            } 
        } 
    } 
    strcpy(out_step_interval, step_inteval);
    //printf("step =%s\n", step_inteval);
}



