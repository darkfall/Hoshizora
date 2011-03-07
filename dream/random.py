import wx
from addperson import addperson
from utilities import *
from wxPython.lib.printout import PrintTable

ADD=wx.NewId()
EDIT=wx.NewId()
CLOSE=wx.NewId()
DISPLAY=wx.NewId()
DELETE=wx.NewId()
PRINT=wx.NewId()


class managepersons(wx.Dialog):
    def __init__(self,parent,id,title, **kwds):
        # begin wxGlade: manage_users.__init__
        self.idu = ''
        self.title = "Manage Persons"
        self.parent=parent
        kwds["style"] = wx.RESIZE_BORDER
        wx.Dialog.__init__(self, parent,id,title, **kwds)
        
        self.lctsup = wx.ListCtrl(self, DISPLAY,
            style=wx.LC_REPORT|wx.LC_SINGLE_SEL, size=(1,150))

        self.lctsup.InsertColumn(0,'Title')
        self.lctsup.InsertColumn(1,'First Name')
        self.lctsup.InsertColumn(2,'Last Name')
        self.lctsup.InsertColumn(3,'Profession')
        
       
        self.butadd = wx.Button(self, ADD, "Add")
        self.butedit = wx.Button(self, EDIT, "Edit")
        self.butdelete = wx.Button(self, DELETE, "Delete")
        self.butprint = wx.Button(self, PRINT, "Print")
        self.butclose = wx.Button(self, CLOSE, "Close")
        self.refresh()
        self.__do_layout()




    def __do_layout(self):

        boxa = wx.BoxSizer(wx.VERTICAL)
        boxt = wx.BoxSizer(wx.VERTICAL)
        boxb = wx.BoxSizer(wx.HORIZONTAL)
        boxt.Add(self.lctsup, 1, wx.EXPAND)
        boxb.Add(self.butadd, 0, 
         wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL)
        boxb.Add(self.butedit, 0, 
         wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL)
        boxb.Add(self.butdelete, 0, 
         wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL)
        boxb.Add(self.butprint, 0, 
         wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL)
        boxb.Add(self.butclose, 0, 
         wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL)
        boxa.Add(boxt,1, wx.EXPAND)
        boxa.Add(boxb,0, wx.EXPAND)
        self.SetAutoLayout(1)
        self.SetSizer(boxa)
        boxa.Fit(self)
        boxa.SetSizeHints(self)
        self.Layout()
        wx.EVT_BUTTON(self, ADD, self.add)
        wx.EVT_BUTTON(self, EDIT, self.edit)
        wx.EVT_BUTTON(self, DELETE, self.delete)
        wx.EVT_LIST_ITEM_SELECTED(self, DISPLAY, self.display)
        wx.EVT_BUTTON(self, CLOSE, self.close)
        wx.EVT_BUTTON(self, PRINT, self.onprint)
 
    def close(self, event):
     self.Destroy()

    def refresh(self):
        self.lctsup.DeleteAllItems()
        x = 0
        self.personList = getpersons() #external program
        for k in self.personList:
            self.lctsup.InsertStringItem(x,k['salutation'])
            self.lctsup.SetStringItem(x,1,k['fname'])
            self.lctsup.SetStringItem(x,2,str(k['lname']))
            self.lctsup.SetStringItem(x,3,str(k['profession']))
            x += 1
            
    def add(self, event):
        aperson = addperson(self, -1, "Add New Person",
                                 False, 0)
    val=aperson.ShowModal()
    if val:
        self.refresh()
            
    def edit(self, event):
        if self.idu:
            aperson = addperson(self, -1, "Edit Person",
                                     True, self.idu)
            val=aperson.ShowModal()
            if val:
                self.refresh()
                
    def delete(self, event):
        if self.idu:
            mg = "Are You Sure You want to delete %s" %(getname(self.idu))
            msg = wx.MessageDialog(self,mg,"Warning",wx.OK|wx.CANCEL)
            res = msg.ShowModal()
            msg.Destroy()
            if res == wx.ID_OK:
                tes = deleteperson(self.idu) #external program
                if tes:
                    #display error message
                    error = wx.MessageDialog(self,"Delete Failed","Error",
                       wx.OK)
                    error.ShowModal()
                    error.Destroy()
                else:
                    self.refresh()
                    
    def display(self,event):
        curitem = event.m_itemIndex
        fitem = self.lctsup.GetItem(curitem,1).GetText()
        litem = self.lctsup.GetItem(curitem,2).GetText()
        self.idu = getid(fitem,litem)
        


    def onprint(self,evt):
        data = []
        data.append(["Salutation","First Name","Last Name","Profession"])
        for k in self.personList:
            data.append([k['salutation'],k['fname'],k['lname'],
         k['profession']])
        prt = PrintTable(self.parent)
        prt.data = data[1:]
        prt.left_margin = .2
        prt.set_column = [ 2,2,2,2]
        prt.label = data[0]
        prt.top_margin = 1
        prt.SetLandscape()
        prt.SetHeader("Person List Report", size = 30)
        prt.SetFooter("Page No", colour = wx.NamedColour('RED'), type ="Num")
        prt.SetRowSpacing(10,10)
        prt.Print()


class app(wx.App):
    def OnInit(self):
        frame=managepersons(None,-1,'')
        self.SetTopWindow(frame)
        frame.Show()
        return 1



if __name__ == "__main__":
    prog = app(0)
    prog.MainLoop()