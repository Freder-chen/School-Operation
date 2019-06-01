// systemAdminIndex.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    item_data: [
      { text: "录入新读者", icon_type: "add"},
      { text: "删除读者", icon_type: "delete" },
      { text: "修改读者信息", icon_type: "editor" },
      { text: "录入图书管理员", icon_type: "add" },
      { text: "删除图书管理员", icon_type: "delete" },
      { text: "修改管理员信息", icon_type: "editor" },
      { text: "查看读者信息", icon_type: "activity" },
      { text: "查看管理员信息信息", icon_type: "activity" },
      { text: "管理员中心", icon_type: "homepage" }
    ]
  },

  itemClick: function(e) {
    switch(e.currentTarget.id) {
      case "item_0": wx.navigateTo({ url: '../adminAddReader/adminAddReader' }); break;
      case "item_1": wx.navigateTo({ url: '../adminOutReader/adminOutReader' }); break;
      case "item_2": wx.navigateTo({ url: '../adminAlterReader/adminAlterReader' }); break;
      case "item_3": wx.navigateTo({ url: '../adminAddAd/adminAddAd' }); break;
      case "item_4": wx.navigateTo({ url: '../adminOutAd/adminOutAd' }); break;
      case "item_5": wx.navigateTo({ url: '../adminAlterAd/adminAlterAd' }); break;
      case "item_6": wx.navigateTo({ url: '../adminSearchReader/adminSearchReader' }); break;
      case "item_7": wx.navigateTo({ url: '../adminSearchAd/adminSearchAd' }); break;
      case "item_8": wx.navigateTo({ url: '../adminPersonal/adminPersonal' }); break;
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