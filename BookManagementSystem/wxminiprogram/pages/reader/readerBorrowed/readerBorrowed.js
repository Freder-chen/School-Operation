// pages/reader/readerBorrowed/readerBorrowed.js

const { $Message } = require('../../../lib/base/index');
const app = getApp();

Page({

  /**
   * 页面的初始数据
   */
  data: {
    tableKeyList: ['序号', '借阅号', '图书名', '借书日期', '还书日期'],
    tableDataList: []
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    var that = this
    wx.request({
      url: app.globalData.post_url + 'find-borrowed-self',
      method: "POST",
      header: app.globalData.header,
      data: {},
      success: function (res) {
        let jdata = res.data;
        if (jdata.status == "success") {
          that.setData({ tableDataList: jdata.data })
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