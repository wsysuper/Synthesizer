#pragma once

#define NULL 0

#define FALSE 0
#define TRUE 1 

#define ERRORVAL -9999

typedef double ELEMTYPE;

typedef struct _ElemNode
{
	_ElemNode* pRight;
	_ElemNode* pDown;
    ELEMTYPE  val;
}ElemNode;

typedef struct
{
	   int nRow,nCol;
 ElemNode* pFirst;
}HeadNode;

class SMatrixChild
{
public:
   ELEMTYPE& operator[](int iCol);
	HeadNode Head;
	     int nRow;
};

class SMatrix
{
public:
	       SMatrix();                                                  //Create a 0��0 Matrix
	       SMatrix(int iRow,int iCol);                                 //create a nRow��nCol Matrix
	       SMatrix(int iRow,int iCol,ELEMTYPE Val);                    //create a nRow��nCol Matrix,initals every menber as Val
	       SMatrix(const SMatrix& other);                              //create a Matrix,size like other
          ~SMatrix();                                                  //����        
                      
       SMatrix& operator =  (const SMatrix& other);
	   SMatrix  operator +  (const SMatrix& other) const;
	   SMatrix& operator += (const SMatrix& other);
	   SMatrix  operator -  (const SMatrix& other) const;
       SMatrix& operator -= (const SMatrix& other);
	   SMatrix  operator *  (const SMatrix& other) const;
	   SMatrix  operator *  (double value)         const;
friend SMatrix  operator *  (double value ,const SMatrix& other);
       
   SMatrix  T(void)const;                                              //����ת�þ���
   SMatrix  Invert(void);                                              //�����������ȫѡ��Ԫ��

      bool T_Self(void);                                               //ԭ��ת��
	  bool Invert_Self(void)const;                                     //ȫѡ��Ԫ��ԭ������

       int nRow()const;                                                //Row
	   int nCol()const;                                                //Col


	 ELEMTYPE& GetElem(int iRow,int iCol)const;                       //�õ�iRow,iCorλ�õ�ֵ������*.[iRow][iCol]���� 
 SMatrixChild& operator [] (int iRow);                                //��������
	      bool Paste(SMatrix& other,int top,int left)const;           //��other�������ϽǶ��뵽left��topλ�ã���ֵ����ǰ����
      ELEMTYPE Addup(int iPower)const;                                //��Ԫ�ص�power�η��ۼ�
	  ELEMTYPE Distance_E(SMatrix& other)const;                       //ŷ�Ͼ���


 	  bool ExchangeRows(int iRow1,int iRo2)const;                      //������ 
 	  bool ExchangeCols(int iCol1,int iCol2)const;                     //������ 
	  bool MakeUnit()const;                                            //ת��Ϊ��λ����
	  bool Zero()const;                                                //����
	  bool Fabs()const;                                                //����ֵȡ����ֵ


	  bool ReSize(int iRow,int iCol)const;                             //�ض����С,������ֲ�Ϊ0
	  bool ReSize(int iRow,int iCol, ELEMTYPE Val)const;               //�ض����С,������ֲ�ΪVal
      bool AddRows(int in,ELEMTYPE Val)const;                          //ԭ��������n�У��Ӻ��� (n<0������Ϊɾ��)
      bool AddCols(int in,ELEMTYPE Val)const;                          //ԭ��������n�У������� (n<0������Ϊɾ��)
      bool DelRows(int in)const;                                       //ԭ����ɾ����n�У�n����ԭ��������ʱ�����Ϊ 0��0
      bool DelCols(int in)const;                                       //ԭ����ɾ����n�У�n����ԭ��������ʱ�����Ϊ 0��0


   SMatrix GetRowVector(int iRow)const;                                      //�õ�һ��������
   SMatrix GetColVector(int iCol)const;                                      //�õ�һ��������
   SMatrix GetDiagonalVector() const;                                        //�õ��Խ�����(������)
   SMatrix GetPart(int left,int top,int bottom,int right) const;             //�����в��ֹ���һ���¾���      
      bool CutIn2_Vertical(SMatrix& Left,SMatrix& Right,int iCol);           //��icolλ�����г�����������(��߲�����iCol��)
      bool CutIn2_Across(SMatrix& Upper,SMatrix& Lower,int iRow);            //��iRowλ�ú��г�����������(�ϱ߲�����iRow��)
      bool Jointer_Right(SMatrix& other);                                    //��other������ڸþ����ұ�
      bool Jointer_bottom(SMatrix& other);                                   //��other������ڸþ�������
	  bool Jointer_Diagonal(SMatrix& other,ELEMTYPE Val);                    //��other������ڸþ�������,�հײ�����Val����
    
private:
   ElemNode* GetElemP(int iRow,int iCol)const;                               //�õ�ĳλ�ýṹ��ָ��
	HeadNode *pHead;
SMatrixChild myChild;
};
