// pages/bookAdmin/adPersonal/adPersonal.js
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
    phone: "",
    email: "",
  },

  alterBookAdminClick: function () {
    var data = {
      old_password: this.data.oldPassword,
      phone: this.data.phone,
      email: this.data.email,
    }
    if (this.data.newPassword != "") {
      if (this.data.againPassword != this.data.newPassword) {
        $Message({ content: "请确认密码一致" });
      } else {
        data.new_password = this.data.newPassword;
      }
    }
    wx.request({
      url: app.globalData.post_url + 'set-book-admin-self',
      method: "POST",
      header: app.globalData.header,
      data: data,
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
  bindPhoneInput: function ({ detail }) { this.setData({ phone: detail.detail.value }) },
  bindEmailInput: function ({ detail }) { this.setData({ email: detail.detail.value }) },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    var that = this
    wx.request({
      url: app.globalData.post_url + 'get-self-wx',
      method: "POST",
      header: app.globalData.header,
      data: {},
      success: function (res) {
        let jdata = res.data;
        if (jdata.status == "success") {
          that.setData({
            phone: jdata.data.phone,
            email: jdata.data.email,
          })
        } else {
          $Message({ content: jdata.status });
        }
      },
      fail: function (res) {
        $Message({ content: '异常' });
      },
    })
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