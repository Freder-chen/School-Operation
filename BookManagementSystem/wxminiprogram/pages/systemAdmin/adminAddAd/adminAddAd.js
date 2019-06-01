// pages/adminAddAd/adminAddAd.js

const { $Message } = require('../../../lib/base/index');
const app = getApp();

Page({

  /**
   * 页面的初始数据
   */
  data: {
    user_name: "",
    // user data
    id: "",
    name: "",
    phone: "",
    email: "",
    password: "",
  },

  addBookAdminClick: function () {
    wx.request({
      url: app.globalData.post_url + 'add-book-admin',
      method: "POST",
      header: app.globalData.header,
      data: {
        id: this.data.id,
        name: this.data.name,
        phone: this.data.phone,
        email: this.data.email,
        password: this.data.password
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

  bindIdInput: function ({ detail }) { this.setData({ id: detail.detail.value }) },
  bindNameInput: function ({ detail }) { this.setData({ name: detail.detail.value }) },
  bindPhoneInput: function ({ detail }) { this.setData({ phone: detail.detail.value }) },
  bindEmailInput: function ({ detail }) { this.setData({ email: detail.detail.value }) },
  bindPasswordInput: function ({ detail }) { this.setData({ password: detail.detail.value }) },

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