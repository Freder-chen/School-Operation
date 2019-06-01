// pages/reader/readerIndex/readerIndex.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    item_data: [
      { text: "基本资料", icon_type: "businesscard" },
      { text: "修改资料", icon_type: "editor" },
      { text: "修改密码", icon_type: "editor" },
      { text: "在借图书", icon_type: "activity" },
      { text: "借阅记录", icon_type: "activity" },
    ]
  },

  itemClick: function (e) {
    switch (e.currentTarget.id) {
      case "item_0": wx.navigateTo({ url: '../readerPersonal/readerPersonal' }); break;
      case "item_1": wx.navigateTo({ url: '../readerAlterData/readerAlterData' }); break;
      case "item_2": wx.navigateTo({ url: '../readerAlterPaswd/readerAlterPaswd' }); break;
      case "item_3": wx.navigateTo({ url: '../readerBorrowing/readerBorrowing' }); break;
      case "item_4": wx.navigateTo({ url: '../readerBorrowed/readerBorrowed' }); break;
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