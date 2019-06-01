// pages/bookAdmin/bookAdminIndex/bookAdminIndex.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    item_data: [
      { text: "读者借书", icon_type: "add" },
      { text: "读者还书", icon_type: "delete" },
      { text: "新书入库", icon_type: "add" },
      { text: "图书出库", icon_type: "delete" },
      { text: "修改图书信息", icon_type: "editor" },
      { text: "查阅借阅记录", icon_type: "search" },
      { text: "查阅图书信息", icon_type: "search" },
      { text: "管理员中心", icon_type: "homepage" }
    ]
  },

  itemClick: function (e) {
    switch (e.currentTarget.id) {
      case "item_0": wx.navigateTo({ url: '../adBorrowBook/adBorrowBook' }); break;
      case "item_1": wx.navigateTo({ url: '../adReturnBook/adReturnBook' }); break;
      case "item_2": wx.navigateTo({ url: '../adAddBook/adAddBook' }); break;
      case "item_3": wx.navigateTo({ url: '../adOutBook/adOutBook' }); break;
      case "item_4": wx.navigateTo({ url: '../adAlterBook/adAlterBook' }); break;
      case "item_5": wx.navigateTo({ url: '../adCheckBook/adCheckBook' }); break;
      case "item_6": wx.navigateTo({ url: '../adSearchBook/adSearchBook' }); break;
      case "item_7": wx.navigateTo({ url: '../adPersonal/adPersonal' }); break;
    }
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})