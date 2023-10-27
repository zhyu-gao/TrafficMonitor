#include "stdafx.h"
#include "ListCtrlEx.h"

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

CListCtrlEx::CListCtrlEx()
{
}


CListCtrlEx::~CListCtrlEx()
{
}

void CListCtrlEx::Edit(int row, int col)
{
    EnsureVisible(row, FALSE);				//�༭һ��ʱȷ�����пɼ�
    m_editing = true;

    m_edit_row = row;
    m_edit_col = col;
    CRect item_rect;
    GetSubItemRect(row, col, LVIR_LABEL, item_rect);	//ȡ������ľ���

    CString text = GetItemText(row, col);		//ȡ�����������

    m_item_edit.SetWindowText(text);		//�������������ʾ���༭����
    m_item_edit.ShowWindow(SW_SHOW);		//��ʾ�༭��
    m_item_edit.MoveWindow(item_rect);		//���༭���ƶ����������棬������������
    m_item_edit.SetFocus();					//ʹ�༭��ȡ�ý���
    m_item_edit.SetSel(0, -1);
}

void CListCtrlEx::SetEditColMethod(eEditColMethod method)
{
    m_edit_col_method = method;
}

void CListCtrlEx::SetEditableCol(const std::initializer_list<int>& paras)
{
    m_edit_cols = paras;
}

void CListCtrlEx::EndEdit()
{
    if (m_editing)
    {
        if (m_edit_row >= 0 && m_edit_row < GetItemCount())
        {
            CString str;
            m_item_edit.GetWindowText(str);	//ȡ�ñ༭�������
            SetItemText(m_edit_row, m_edit_col, str);	//�������ݸ��µ�CListCtrl��
        }
        m_item_edit.ShowWindow(SW_HIDE);//���ر༭��
        m_editing = false;
    }
}

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
    ON_EN_KILLFOCUS(IDC_ITEM_EDITBOX, &CListCtrlEx::OnEnKillfocusEdit1)
    ON_NOTIFY_REFLECT_EX(NM_DBLCLK, &CListCtrlEx::OnNMDblclk)
    ON_NOTIFY_REFLECT(LVN_BEGINSCROLL, &CListCtrlEx::OnLvnBeginScroll)
    ON_MESSAGE(WM_TABLET_QUERYSYSTEMGESTURESTATUS, &CListCtrlEx::OnTabletQuerysystemgesturestatus)
END_MESSAGE_MAP()


void CListCtrlEx::OnEnKillfocusEdit1()
{
    //���ı��༭�ؼ��ؼ�ʧȥ����ʱ��Ӧ
    EndEdit();
}



void CListCtrlEx::PreSubclassWindow()
{
    // TODO: �ڴ����ר�ô����/����û���
    m_item_edit.Create(WS_BORDER | ES_AUTOHSCROLL, CRect(), this, IDC_ITEM_EDITBOX);
    m_item_edit.SetFont(GetFont());

    CListCtrl::PreSubclassWindow();
}


BOOL CListCtrlEx::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    //���˫��������Ҫ�༭����
    if (m_edit_col_method == EC_ALL
        || (m_edit_col_method == EC_SPECIFIED && m_edit_cols.find(pNMItemActivate->iSubItem) != m_edit_cols.end()))
    {
        Edit(pNMItemActivate->iItem, pNMItemActivate->iSubItem);
        *pResult = 0;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


void CListCtrlEx::OnLvnBeginScroll(NMHDR *pNMHDR, LRESULT *pResult)
{
    // �˹���Ҫ�� Internet Explorer 5.5 ����߰汾��
    // ���� _WIN32_IE ������ >= 0x0560��
    LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    EndEdit();

    *pResult = 0;
}


afx_msg LRESULT CListCtrlEx::OnTabletQuerysystemgesturestatus(WPARAM wParam, LPARAM lParam)
{
    return 0;
}
