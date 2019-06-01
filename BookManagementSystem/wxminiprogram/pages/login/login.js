// pages/login/login.js

const { $Message } = require('../../lib/base/index');
const app = getApp();

Page({

  /**
   * 页面的初始数据
   */
  data: {
    user_name: "",
    user_password: ""
  },

  bindUserNameInput: function ({ detail }) {
    this.setData({ user_name: detail.detail.value })
  },

  bindUserPasswordInput: function ({ detail }) {
    this.setData({ user_password: detail.detail.value })
  },

  login_click: function() {
    wx.request({
      url: app.globalData.post_url + 'login-wx',
      method: "POST",
      header: app.globalData.header,
      data: {
        'username': this.data.user_name,
        'password': this.data.user_password
      },
      success: function (res) {
        app.globalData.header.cookie = res.header['Set-Cookie']
        let jdata = res.data
        if (jdata.status == "success") {
          switch(jdata.type) {
            case "user":
              wx.navigateTo({ url: '../reader/readerIndex/readerIndex' });
              break;
            case "book_admin":
              wx.navigateTo({ url: '../bookAdmin/bookAdminIndex/bookAdminIndex' });
              break;
            case "system_admin":
              wx.navigateTo({ url: '../systemAdmin/systemAdminIndex/systemAdminIndex' });
              break;
            default:
              $Message({ content: '服务器异常' });
              break;
          }
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
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {},

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {},

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {},

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {},

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {},

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {},

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {},

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {}
})