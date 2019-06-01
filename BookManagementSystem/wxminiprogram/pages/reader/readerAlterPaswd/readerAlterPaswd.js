// pages/reader/readerAlterPaswd/readerAlterPaswd.js
const { $Message } = require('../../../lib/base/index');
const app = getApp();

Page({

  /**
   * 页面的初始数据
   */
  data: {
    oldPassword: "",
    newPassword: "",
    againPassword: "",
  },

  alterSystemAdminClick: function () {
    if (this.data.againPassword != this.data.newPassword) {
      $Message({ content: "请确认密码一致" });
      return;
    }
    wx.request({
      url: app.globalData.post_url + 'set-self-password',
      method: "POST",
      header: app.globalData.header,
      data: {
        old_password: this.data.oldPassword,
        new_password: this.data.newPassword
      },
      success: function (res) {
        let jdata = res.data
        $Message({ content: jdata.status });
      },
      fail: function (res) {
        $Message({ content: '异常' });
      },
    })
  },

  bindOldPasswordInput: function ({ detail }) { this.setData({ oldPassword: detail.detail.value }) },
  bindNewPasswordInput: function ({ detail }) { this.setData({ newPassword: detail.detail.value }) },
  bindAgainPasswordInput: function ({ detail }) { this.setData({ againPassword: detail.detail.value }) },

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