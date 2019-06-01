// pages/adminSearchReader/adminSearchReader.js
const { $Message } = require('../../../lib/base/index');
const app = getApp();

Page({

  /**
   * 页面的初始数据
   */
  data: {
    search: "",

    userKeyList: [
      '借阅号', '用户名', '学院', '专业', '电话',
      '邮件', '借阅上限', '借阅期限', '在借数量'
    ],
    userDataList: []
  },

  searchClick: function() {
    var that = this
    wx.request({
      url: app.globalData.post_url + 'find-user',
      method: "POST",
      header: app.globalData.header,
      data: {
        find_value: that.data.search,
      },
      success: function (res) {
        let jdata = res.data;
        if (jdata.status == "success") {
          that.setData({
            userDataList: jdata.data,
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

  bindSearchInput: function ({ detail }) { this.setData({ search: detail.detail.value }) },

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